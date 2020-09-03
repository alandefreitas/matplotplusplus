#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::string text = fileread("shakespeare_sonnets.txt");
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);

    std::vector<std::string> black_list =
        tokenize(fileread("en_blacklist.txt"));

    wordcloud(text, black_list);
    title("Shakespeare's sonnets");

    show();
    return 0;
}