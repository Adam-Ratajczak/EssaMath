#include "include/Generator.hpp"
#include <iostream>

namespace Essa::Math{
   namespace lexer{
         generator::generator()
         : base_itr_(0)
         , s_itr_   (0)
         , s_end_   (0)
         {
            clear();
         }

         void generator::clear()
         {
            base_itr_ = 0;
            s_itr_    = 0;
            s_end_    = 0;
            token_list_.clear();
            token_itr_ = token_list_.end();
            store_token_itr_ = token_list_.end();
         }

         bool generator::process(const std::string& str)
         {
            base_itr_ = str.data();
            s_itr_    = str.data();
            s_end_    = str.data() + str.size();

            eof_token_.set_operator(token_t::e_eof,s_end_,s_end_,base_itr_);
            token_list_.clear();

            while (!is_end(s_itr_))
            {
               scan_token();

               if (!token_list_.empty() && token_list_.back().is_error())
                  return false;
            }

            return true;
         }

         bool generator::empty() const
         {
            return token_list_.empty();
         }

         std::size_t generator::size() const
         {
            return token_list_.size();
         }

         void generator::begin()
         {
            token_itr_ = token_list_.begin();
            store_token_itr_ = token_list_.begin();
         }

         void generator::store()
         {
            store_token_itr_ = token_itr_;
         }

         void generator::restore()
         {
            token_itr_ = store_token_itr_;
         }

         generator::token_t& generator::next_token()
         {
            if (token_list_.end() != token_itr_)
            {
               return *token_itr_++;
            }
            else
               return eof_token_;
         }

         generator::token_t& generator::peek_next_token()
         {
            if (token_list_.end() != token_itr_)
            {
               return *token_itr_;
            }
            else
               return eof_token_;
         }

         generator::token_t& generator::operator[](const std::size_t& index)
         {
            if (index < token_list_.size())
               return token_list_[index];
            else
               return eof_token_;
         }

         generator::token_t generator::operator[](const std::size_t& index) const
         {
            if (index < token_list_.size())
               return token_list_[index];
            else
               return eof_token_;
         }

         bool generator::finished() const
         {
            return (token_list_.end() == token_itr_);
         }

         void generator::insert_front(token_t::token_type tk_type)
         {
            if (
                 !token_list_.empty() &&
                 (token_list_.end() != token_itr_)
               )
            {
               token_t t = *token_itr_;

               t.type     = tk_type;
               token_itr_ = token_list_.insert(token_itr_,t);
            }
         }

         std::string generator::substr(const std::size_t& begin, const std::size_t& end) const
         {
            const details::char_cptr begin_itr = ((base_itr_ + begin) < s_end_) ? (base_itr_ + begin) : s_end_;
            const details::char_cptr end_itr   = ((base_itr_ + end  ) < s_end_) ? (base_itr_ + end  ) : s_end_;

            return std::string(begin_itr,end_itr);
         }

         std::string generator::remaining() const
         {
            if (finished())
               return "";
            else if (token_list_.begin() != token_itr_)
               return std::string(base_itr_ + (token_itr_ - 1)->position, s_end_);
            else
               return std::string(base_itr_ + token_itr_->position, s_end_);
         }

         bool generator::is_end(details::char_cptr itr) const
         {
            return (s_end_ == itr);
         }

         bool generator::is_comment_start(details::char_cptr itr) const
         {
            if(details::disable_comments)
               return false;
            const char_t c0 = *(itr + 0);
            const char_t c1 = *(itr + 1);

            if ('#' == c0)
               return true;
            else if (!is_end(itr + 1))
            {
               if (('/' == c0) && ('/' == c1)) return true;
               if (('/' == c0) && ('*' == c1)) return true;
            }
            return false;
         }

         void generator::skip_whitespace()
         {
            while (!is_end(s_itr_) && details::is_whitespace(*s_itr_))
            {
               ++s_itr_;
            }
         }

         void generator::skip_comments()
         {
            if(details::disable_comments)
               return;
            // The following comment styles are supported:
            // 1. // .... \n
            // 2. #  .... \n
            // 3. /* .... */
            struct test
            {
               static bool comment_start(const char_t c0, const char_t c1, int& mode, int& incr)
               {
                  mode = 0;
                  if      ('#' == c0)    { mode = 1; incr = 1; }
                  else if ('/' == c0)
                  {
                     if      ('/' == c1) { mode = 1; incr = 2; }
                     else if ('*' == c1) { mode = 2; incr = 2; }
                  }
                  return (0 != mode);
               }

               static bool comment_end(const char_t c0, const char_t c1, int& mode)
               {
                  if (
                       ((1 == mode) && ('\n' == c0)) ||
                       ((2 == mode) && ( '*' == c0) && ('/' == c1))
                     )
                  {
                     mode = 0;
                     return true;
                  }
                  else
                     return false;
               }
            };

            int mode      = 0;
            int increment = 0;

            if (is_end(s_itr_))
               return;
            else if (!test::comment_start(*s_itr_, *(s_itr_ + 1), mode, increment))
               return;

            details::char_cptr cmt_start = s_itr_;

            s_itr_ += increment;

            while (!is_end(s_itr_))
            {
               if ((1 == mode) && test::comment_end(*s_itr_, 0, mode))
               {
                  ++s_itr_;
                  return;
               }

               if ((2 == mode))
               {
                  if (!is_end((s_itr_ + 1)) && test::comment_end(*s_itr_, *(s_itr_ + 1), mode))
                  {
                     s_itr_ += 2;
                     return;
                  }
               }

                ++s_itr_;
            }

            if (2 == mode)
            {
               token_t t;
               t.set_error(token::e_error, cmt_start, cmt_start + mode, base_itr_);
               token_list_.push_back(t);
            }
         }

         void generator::scan_token()
         {
            if (details::is_whitespace(*s_itr_))
            {
               skip_whitespace();
               return;
            }
            else if (is_comment_start(s_itr_))
            {
               skip_comments();
               return;
            }
            else if (details::is_operator_char(*s_itr_))
            {
               scan_operator();
               return;
            }
            else if (details::is_letter(*s_itr_))
            {
               scan_symbol();
               return;
            }
            else if (details::is_digit((*s_itr_)) || ('.' == (*s_itr_)))
            {
               scan_number();
               return;
            }
            else if ('$' == (*s_itr_))
            {
               scan_special_function();
               return;
            }
            else if ('~' == (*s_itr_))
            {
               token_t t;
               t.set_symbol(s_itr_, s_itr_ + 1, base_itr_);
               token_list_.push_back(t);
               ++s_itr_;
               return;
            }
            else
            {
               token_t t;
               t.set_error(token::e_error, s_itr_, s_itr_ + 2, base_itr_);
               token_list_.push_back(t);
               ++s_itr_;
            }
         }

         void generator::scan_operator()
         {
            token_t t;

            const char_t c0 = s_itr_[0];

            if (!is_end(s_itr_ + 1))
            {
               const char_t c1 = s_itr_[1];

               if (!is_end(s_itr_ + 2))
               {
                  const char_t c2 = s_itr_[2];

                  if ((c0 == '<') && (c1 == '=') && (c2 == '>'))
                  {
                     t.set_operator(token_t::e_swap, s_itr_, s_itr_ + 3, base_itr_);
                     token_list_.push_back(t);
                     s_itr_ += 3;
                     return;
                  }
               }

               token_t::token_type ttype = token_t::e_none;

               if      ((c0 == '<') && (c1 == '=')) ttype = token_t::e_lte;
               else if ((c0 == '>') && (c1 == '=')) ttype = token_t::e_gte;
               else if ((c0 == '<') && (c1 == '>')) ttype = token_t::e_ne;
               else if ((c0 == '!') && (c1 == '=')) ttype = token_t::e_ne;
               else if ((c0 == '=') && (c1 == '=')) ttype = token_t::e_eq;
               else if ((c0 == ':') && (c1 == '=')) ttype = token_t::e_assign;
               else if ((c0 == '<') && (c1 == '<')) ttype = token_t::e_shl;
               else if ((c0 == '>') && (c1 == '>')) ttype = token_t::e_shr;
               else if ((c0 == '+') && (c1 == '=')) ttype = token_t::e_addass;
               else if ((c0 == '-') && (c1 == '=')) ttype = token_t::e_subass;
               else if ((c0 == '*') && (c1 == '=')) ttype = token_t::e_mulass;
               else if ((c0 == '/') && (c1 == '=')) ttype = token_t::e_divass;
               else if ((c0 == '%') && (c1 == '=')) ttype = token_t::e_modass;

               if (token_t::e_none != ttype)
               {
                  t.set_operator(ttype, s_itr_, s_itr_ + 2, base_itr_);
                  token_list_.push_back(t);
                  s_itr_ += 2;
                  return;
               }
            }

            if ('<' == c0)
               t.set_operator(token_t::e_lt , s_itr_, s_itr_ + 1, base_itr_);
            else if ('>' == c0)
               t.set_operator(token_t::e_gt , s_itr_, s_itr_ + 1, base_itr_);
            else if (';' == c0)
               t.set_operator(token_t::e_eof, s_itr_, s_itr_ + 1, base_itr_);
            else if ('&' == c0)
               t.set_symbol(s_itr_, s_itr_ + 1, base_itr_);
            else if ('|' == c0)
               t.set_symbol(s_itr_, s_itr_ + 1, base_itr_);
            else
               t.set_operator(token_t::token_type(c0), s_itr_, s_itr_ + 1, base_itr_);

            token_list_.push_back(t);
            ++s_itr_;
         }

         void generator::scan_symbol()
         {
            details::char_cptr initial_itr = s_itr_;

            while (!is_end(s_itr_))
            {
               if (!details::is_letter_or_digit(*s_itr_) && ('_' != (*s_itr_)))
               {
                  if ('.' != (*s_itr_))
                     break;
                  /*
                     Permit symbols that contain a 'dot'
                     Allowed   : abc.xyz, a123.xyz, abc.123, abc_.xyz a123_.xyz abc._123
                     Disallowed: .abc, abc.<white-space>, abc.<eof>, abc.<operator +,-,*,/...>
                  */
                  if (
                       (s_itr_ != initial_itr)                     &&
                       !is_end(s_itr_ + 1)                         &&
                       !details::is_letter_or_digit(*(s_itr_ + 1)) &&
                       ('_' != (*(s_itr_ + 1)))
                     )
                     break;
               }

               ++s_itr_;
            }

            token_t t;
            t.set_symbol(initial_itr,s_itr_,base_itr_);
            token_list_.push_back(t);
         }

         void generator::scan_number()
         {
            /*
               Attempt to match a valid numeric value in one of the following formats:
               (01) 123456
               (02) 123456.
               (03) 123.456
               (04) 123.456e3
               (05) 123.456E3
               (06) 123.456e+3
               (07) 123.456E+3
               (08) 123.456e-3
               (09) 123.456E-3
               (00) .1234
               (11) .1234e3
               (12) .1234E+3
               (13) .1234e+3
               (14) .1234E-3
               (15) .1234e-3
            */

            details::char_cptr initial_itr = s_itr_;
            bool dot_found                 = false;
            bool e_found                   = false;
            bool post_e_sign_found         = false;
            bool post_e_digit_found        = false;
            token_t t;

            while (!is_end(s_itr_))
            {
               if ('.' == (*s_itr_))
               {
                  if (dot_found)
                  {
                     t.set_error(token::e_err_number, initial_itr, s_itr_, base_itr_);
                     token_list_.push_back(t);

                     return;
                  }

                  dot_found = true;
                  ++s_itr_;

                  continue;
               }
               else if ('e' == std::tolower(*s_itr_))
               {
                  const char_t& c = *(s_itr_ + 1);

                  if (is_end(s_itr_ + 1))
                  {
                     t.set_error(token::e_err_number, initial_itr, s_itr_, base_itr_);
                     token_list_.push_back(t);

                     return;
                  }
                  else if (
                            ('+' != c) &&
                            ('-' != c) &&
                            !details::is_digit(c)
                          )
                  {
                     t.set_error(token::e_err_number, initial_itr, s_itr_, base_itr_);
                     token_list_.push_back(t);

                     return;
                  }

                  e_found = true;
                  ++s_itr_;

                  continue;
               }
               else if (e_found && details::is_sign(*s_itr_) && !post_e_digit_found)
               {
                  if (post_e_sign_found)
                  {
                     t.set_error(token::e_err_number, initial_itr, s_itr_, base_itr_);
                     token_list_.push_back(t);

                     return;
                  }

                  post_e_sign_found = true;
                  ++s_itr_;

                  continue;
               }
               else if (e_found && details::is_digit(*s_itr_))
               {
                  post_e_digit_found = true;
                  ++s_itr_;

                  continue;
               }
               else if (('.' != (*s_itr_)) && !details::is_digit(*s_itr_))
                  break;
               else
                  ++s_itr_;
            }

            t.set_numeric(initial_itr, s_itr_, base_itr_);
            token_list_.push_back(t);

            return;
         }

         void generator::scan_special_function()
         {
            details::char_cptr initial_itr = s_itr_;
            token_t t;

            // $fdd(x,x,x) = at least 11 chars
            if (std::distance(s_itr_,s_end_) < 11)
            {
               t.set_error(
                  token::e_err_sfunc,
                  initial_itr, std::min(initial_itr + 11, s_end_),
                  base_itr_);
               token_list_.push_back(t);

               return;
            }

            if (
                 !(('$' == *s_itr_)                       &&
                   (details::imatch  ('f',*(s_itr_ + 1))) &&
                   (details::is_digit(*(s_itr_ + 2)))     &&
                   (details::is_digit(*(s_itr_ + 3))))
               )
            {
               t.set_error(
                  token::e_err_sfunc,
                  initial_itr, std::min(initial_itr + 4, s_end_),
                  base_itr_);
               token_list_.push_back(t);

               return;
            }

            s_itr_ += 4; // $fdd = 4chars

            t.set_symbol(initial_itr, s_itr_, base_itr_);
            token_list_.push_back(t);

            return;
         }

         void generator::scan_string()
         {
            details::char_cptr initial_itr = s_itr_ + 1;
            token_t t;

            if (std::distance(s_itr_,s_end_) < 2)
            {
               t.set_error(token::e_err_string, s_itr_, s_end_, base_itr_);
               token_list_.push_back(t);

               return;
            }

            ++s_itr_;

            bool escaped_found = false;
            bool escaped = false;

            while (!is_end(s_itr_))
            {
               if (!details::is_valid_string_char(*s_itr_))
               {
                  t.set_error(token::e_err_string, initial_itr, s_itr_, base_itr_);
                  token_list_.push_back(t);

                  return;
               }
               else if (!escaped && ('\\' == *s_itr_))
               {
                  escaped_found = true;
                  escaped = true;
                  ++s_itr_;

                  continue;
               }
               else if (!escaped)
               {
                  if ('\'' == *s_itr_)
                     break;
               }
               else if (escaped)
               {
                  if (
                       !is_end(s_itr_) && ('0' == *(s_itr_)) &&
                       ((s_itr_ + 4) <= s_end_)
                     )
                  {
                     const bool x_seperator = ('X' == std::toupper(*(s_itr_ + 1)));

                     const bool both_digits = details::is_hex_digit(*(s_itr_ + 2)) &&
                                              details::is_hex_digit(*(s_itr_ + 3)) ;

                     if (!(x_seperator && both_digits))
                     {
                        t.set_error(token::e_err_string, initial_itr, s_itr_, base_itr_);
                        token_list_.push_back(t);

                        return;
                     }
                     else
                        s_itr_ += 3;
                  }

                  escaped = false;
               }

               ++s_itr_;
            }

            if (is_end(s_itr_))
            {
               t.set_error(token::e_err_string, initial_itr, s_itr_, base_itr_);
               token_list_.push_back(t);

               return;
            }

            if (!escaped_found)
               t.set_string(initial_itr, s_itr_, base_itr_);
            else
            {
               std::string parsed_string(initial_itr,s_itr_);

               if (!details::cleanup_escapes(parsed_string))
               {
                  t.set_error(token::e_err_string, initial_itr, s_itr_, base_itr_);
                  token_list_.push_back(t);

                  return;
               }

               t.set_string(
                    parsed_string,
                    static_cast<std::size_t>(std::distance(base_itr_,initial_itr)));
            }

            token_list_.push_back(t);
            ++s_itr_;

            return;
         }
   }
}

