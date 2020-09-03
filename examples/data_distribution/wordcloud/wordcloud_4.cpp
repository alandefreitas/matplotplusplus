#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::string text = fileread("shakespeare_sonnets.txt");
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);

    std::vector<std::string> black_list =
        tokenize(fileread("en_blacklist.txt"));

    auto [tokens, count] = wordcount(text, black_list);

    std::vector<double> custom_colors = rand(tokens.size(), 0., 100.);

    wordcloud(tokens, count, custom_colors);
    title("Shakespeare's sonnets - Random colors");

    show();
    return 0;
}