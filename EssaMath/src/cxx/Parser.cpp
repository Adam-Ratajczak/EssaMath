#include "include/Parser.hpp"
#include "include/Operators.hpp"


namespace Essa::Math{
   namespace parser_error
   {
    
        type::type()
         : mode(parser_error::e_unknown)
         , line_no  (0)
         , column_no(0)
         {}

      type make_error(const error_mode mode,
                             const std::string& diagnostic,
                             const std::string& src_location)
      {
         type t;
         t.mode         = mode;
         t.token.type   = lexer::token::e_error;
         t.diagnostic   = diagnostic;
         t.src_location = src_location;
         exprtk_debug(("%s\n",diagnostic .c_str()));
         return t;
      }

      type make_error(const error_mode mode,
                             const lexer::token& tk,
                             const std::string& diagnostic,
                             const std::string& src_location)
      {
         type t;
         t.mode         = mode;
         t.token        = tk;
         t.diagnostic   = diagnostic;
         t.src_location = src_location;
         exprtk_debug(("%s\n",diagnostic .c_str()));
         return t;
      }

      std::string to_str(error_mode mode)
      {
         switch (mode)
         {
            case e_unknown : return std::string("Unknown Error");
            case e_syntax  : return std::string("Syntax Error" );
            case e_token   : return std::string("Token Error"  );
            case e_numeric : return std::string("Numeric Error");
            case e_symtab  : return std::string("Symbol Error" );
            case e_lexer   : return std::string("Lexer Error"  );
            case e_helper  : return std::string("Helper Error" );
            case e_parser  : return std::string("Parser Error" );
            default        : return std::string("Unknown Error");
         }
      }

      bool update_error(type& error, const std::string& expression)
      {
         if (
              expression.empty()                         ||
              (error.token.position > expression.size()) ||
              (std::numeric_limits<std::size_t>::max() == error.token.position)
            )
         {
            return false;
         }

         std::size_t error_line_start = 0;

         for (std::size_t i = error.token.position; i > 0; --i)
         {
            const details::char_t c = expression[i];

            if (('\n' == c) || ('\r' == c))
            {
               error_line_start = i + 1;
               break;
            }
         }

         std::size_t next_nl_position = std::min(expression.size(),
                                                 expression.find_first_of('\n',error.token.position + 1));

         error.column_no  = error.token.position - error_line_start;
         error.error_line = expression.substr(error_line_start,
                                              next_nl_position - error_line_start);

         error.line_no = 0;

         for (std::size_t i = 0; i < next_nl_position; ++i)
         {
            if ('\n' == expression[i])
               ++error.line_no;
         }

         return true;
      }

      void dump_error(const type& error)
      {
         printf("Position: %02d   Type: [%s]   Msg: %s\n",
                static_cast<int>(error.token.position),
                Essa::Math::parser_error::to_str(error.mode).c_str(),
                error.diagnostic.c_str());
      }
   }
}
