#include <complex>
#include <iostream>
#include <EssaMath/EssaMath.hpp>
#include <string>

void function_double()
{
   typedef Essa::Math::symbol_table<double> symbol_table_t;
   typedef Essa::Math::expression<double>   expression_t;
   typedef Essa::Math::parser<double>       parser_t;

   auto foo = [](symbol_table_t& _table, parser_t& _parser, std::string _str) -> expression_t{
      expression_t _expr;
      _expr.register_symbol_table(_table);
      _parser.compile(_str, _expr);

      return _expr;
   };

   double x = 0, y = 0, r = 0, theta = 0;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);
   symbol_table.add_constants();

   parser_t parser;
   
   auto _expr = foo(symbol_table, parser, "asin(2*x) - 2*cosh(x)*atan(x)");

   auto _result = Essa::Math::exponentialize(_expr);
   std::cout << _result.to_string() << "\n";

   for(x = -0.5; x <= 0.5; x += 0.1){
      std::cout << _expr.value() << "\t\t" << _result.value() << "\n";
   }
}
void function_complex()
{
   typedef Essa::Math::symbol_table<std::complex<double>> symbol_table_t;
   typedef Essa::Math::expression<std::complex<double>>   expression_t;
   typedef Essa::Math::parser<std::complex<double>>       parser_t;

   std::complex<double> x, y, a, b, c, d;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);
   symbol_table.add_variable("a",a);
   symbol_table.add_variable("b",b);
   symbol_table.add_variable("c",c);
   symbol_table.add_variable("d",d);
   symbol_table.add_constants();

   expression_t expression, expression2;
   expression.register_symbol_table(symbol_table);
   expression2.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile("(a+b+c)^2", expression);
   parser.compile("exp (a + b*%i)", expression2);

   std::cout << "Expr error: " <<parser.error() << "\n";
   std::cout << "Expr: " << expression.to_string() << "\n";

   auto _result = Essa::Math::expandwrt_factored(expression, {"a", "b"});
   std::cout << "Result: " << _result.to_string() << "\t\t" << Essa::Math::nterms(_result) << "\n";
   auto _result2 = Essa::Math::isolate(_result, "c");
   std::cout << "Result: " << _result2.to_string() << "\t\t" << Essa::Math::nterms(_result2) << "\n";
   // auto _result4 = Essa::Math::partition(_result, "b");
   // std::cout << "Result: " << _result4.first.to_string() << "\t\t" << Essa::Math::nterms(_result4.first) << "\n";
   // std::cout << "Result: " << _result4.second.to_string() << "\t\t" << Essa::Math::nterms(_result4.second) << "\n";
   auto _result5 = Essa::Math::demoivre(expression2);
   std::cout << "Result: " << _result5.to_string() << "\n";

   // for(x = 2.0; x.real() <= 4.0; x += 0.1, y += 0.1){
   //    std::cout << expression.value() << "\t\t" << _result.value() << "\t\t" << _result2.value() << "\n";
   // }
}

int main(int argc, char **argv)
{
   Essa::Math::init_math(argc, argv);

   function_double();
   function_complex();

   Essa::Math::free_math();

    return 0;
}