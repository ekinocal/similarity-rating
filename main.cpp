#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> //C++17
#include <vector>
#include <cstdlib>
#include <map>
#include <utility>
#include <set>
#include <algorithm>
#include <chrono>


using file_compare = bool(&)(const std::filesystem::path&);

// get path of compared files
std::string get_destination_path(const std::string& output_text, file_compare func)
{
    std::string path;
    while (true)
    {
        std::cout << output_text;
        getline(std::cin, path);
        if (std::filesystem::exists(path) && func(path)) break;
        std::cout << "Entered a wrong path, please enter the path again!\n";
    }
    return path;
}

// tokenize strings
std::set<std::string> tokenize(const std::string& line, char delimiter = ' ')
{
    std::stringstream ss{ line };
    std::set<std::string> svec;
    std::string temp;
    while(getline(ss, temp, delimiter))
    {
        svec.emplace(temp);
    }
    return svec;
}

// calculate accuracy
double accuracy(const std::set<std::string>& cmp1, const std::set<std::string>& cmp2)
{
    int accuracy = 0;

    for (const auto& item1 : cmp1)
    {
        for (const auto& item2: cmp2)
        {
            if(item1 == item2)
            {
                ++accuracy;
                break;
            }
        }
    }

    return (static_cast<double>(accuracy) / std::max(cmp1.size(), cmp2.size())) * 100.;
}

int main()
{
    // define excluded words
    std::set<std::string> stop_words{ "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now" };
    const std::string directory_path{ get_destination_path("Enter path of documents : ", std::filesystem::is_directory) };
    std::vector<std::ifstream> document_files;

    for (const auto& item : std::filesystem::directory_iterator{ directory_path })
    {
        if (item.is_regular_file())
        {
            document_files.emplace_back(item.path());
            if (!document_files.back().is_open())
            {
                std::cout << "Failed while opening : " << item.path().filename() << "\n";
                document_files.pop_back();
            }
        }
    }

    if(document_files.empty())
    {
        std::cout << "Please enter an valid directory at least contains 1 document to compare with plagiarism file!\n";
        std::exit(1);
    }

    const std::string plagiarism_file_path{ get_destination_path("Enter path of plagiarism file : ", std::filesystem::is_regular_file) };

    std::ifstream plagiarism_file{ plagiarism_file_path };
    if (!plagiarism_file.is_open())
    {
        std::cout << "Please enter a valid plagiarism file!\n";
        std::exit(1);
    }


    // start timer
    auto start_timer = std::chrono::high_resolution_clock::now();

    std::vector<std::string> target_lines;
    std::string line;
    while (getline(plagiarism_file, line, '.'))
    {
        std::string result;
        std::string transform_result;
        std::transform(line.begin(), line.end(), std::back_inserter(transform_result),[](auto val)
        {
            return std::tolower(val);
        });
        std::unique_copy(transform_result.begin(), transform_result.end(), std::back_inserter(result), [](auto val1, auto val2)
        {
            return std::isspace(val1) && std::isspace(val2);
        });
        result.erase(std::remove_if(result.begin(), result.end(), [](auto val)
        {
            return std::ispunct(val);
        }), result.end());
        target_lines.emplace_back(result);
    }


    // transform destination texts
    std::vector<std::vector<std::string>> source_documents;
    for (auto& item: document_files)
    {
        source_documents.emplace_back(std::vector<std::string>{});
        while (getline(item, line, '.'))
        {
            std::string result;
            std::string transform_result;
            std::transform(line.begin(), line.end(), std::back_inserter(transform_result), [](auto val)
            {
                return std::tolower(val);
            });
            std::unique_copy(transform_result.begin(), transform_result.end(), std::back_inserter(result), [](auto val1, auto val2)
            {
                return std::isspace(val1) && std::isspace(val2);
            });
            result.erase(std::remove_if(result.begin(), result.end(), [](auto val)
            {
                return std::ispunct(val);
            }), result.end());
            source_documents.back().emplace_back(result);
        }
    }

    std::vector<std::pair<double, std::map<std::string, double>>> most_similar_sentences_in_documents;
    std::set<std::string> target_document_set;
    for (const auto& item : target_lines)
    {
        auto words{ tokenize(item) };
        std::for_each(stop_words.begin(), stop_words.end(), [&words](auto val)
        {
            words.erase(val);
        });
        target_document_set.insert(words.begin(), words.end());
    }
    for (const auto& document : source_documents)
    {
        std::set<std::string> document_set;
        std::map<std::string, double> similarity_lines;
        for (const auto& line : document)
        {
            auto tokenized = tokenize(line);
            std::for_each(stop_words.begin(), stop_words.end(), [&tokenized](auto val)
            {
                tokenized.erase(val);
            });
            document_set.insert(tokenized.begin(), tokenized.end());
            for (const auto& item : target_lines)
            {
                auto words{ tokenize(item) };
                std::for_each(stop_words.begin(), stop_words.end(), [&words](auto val)
                {
                    words.erase(val);
                });
                if (auto iter = similarity_lines.find(item); iter != similarity_lines.end()) //
                {
                    if(auto new_accuracy = accuracy(words, tokenized); iter->second < new_accuracy)
                    {
                        similarity_lines[item] = new_accuracy;
                    }
                }
                else
                {
                    similarity_lines[item] = accuracy(words, tokenized);
                }
            }
        }
        most_similar_sentences_in_documents.emplace_back(accuracy(target_document_set, document_set), similarity_lines);
    }

    // stop timer and print duration
    auto stop_timer = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_timer - start_timer);
    std::cout << std::endl;
    std::cout << "Finished within " << duration.count() << " milliseconds  " << std::endl;

    int cnt{};
    for (const auto& item : most_similar_sentences_in_documents)
    {
        if(cnt == 0){
            std::cout <<"--------------------------------------------------";
        }
        std::cout <<"\n";
        std::cout <<"Document #" << cnt++ << " Similarity Rate => " << item.first << "%\n";
        std::vector<std::pair<std::string, double>> vec;

        for (const auto &map_item : item.second)
        {
            vec.emplace_back(map_item.first, map_item.second);
        }
        std::sort(vec.begin(), vec.end(), [](auto val, auto val2)
        {
            return val.second > val2.second;
        });
        std::for_each_n(vec.begin(), 5, [](auto val)
        {
            std::cout << "-" << val.first << " => [RATE] : " << val.second << "%\n";
        });
        std::cout <<"\n";
        std::cout <<"--------------------------------------------------";
        std::cout <<"\n";
    }
}
