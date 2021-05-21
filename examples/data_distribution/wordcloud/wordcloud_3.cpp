#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::string text = fileread("shakespeare_sonnets.txt");
    std::transform(text.begin(), text.end(), text.begin(),
                   [](char c) { return static_cast<char>(std::tolower(static_cast<int>(c))); });

    std::vector<std::string> black_list =
        tokenize(fileread("en_blacklist.txt"));

    std::vector<std::string> text_tokens = tokenize(text);
    auto it = std::remove_if(text_tokens.begin(), text_tokens.end(),
                             [](const std::string &s) { return s.size() < 5; });
    text_tokens.erase(it, text_tokens.end());

    auto [tokens, count] = wordcount(text_tokens, black_list);

    wordcloud(tokens, count);
    title("Shakespeare's sonnets - Big words");

    matplot::show();
    return 0;
}