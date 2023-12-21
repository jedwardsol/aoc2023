#include <print>

#include "include/getdata.h"

void withVariant    (std::vector<std::string>   const lines);
void withInheritance(std::vector<std::string>   const lines);

int main()
try
{
    auto lines = getDataLines();

    std::print("withVariant\n");
    withVariant(lines);

    std::print("\n\nwithVariant\n");
    withInheritance(lines);

}
catch(std::exception const &e)
{
    std::print("{}",e.what());
}





#include <sstream>

// --------------------------
std::istringstream testInput{
R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output
)"};
