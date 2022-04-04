Which language did we choose to implement the project?

The programming language we have chosen is C++17 .

How does the program work?

Defining stop words.

std​::​set​<​std​::​string​> stop_words{ ​"i"​, ​"me"​, ​"my"​, ​"myself"​, "​we"​, ​"our",​ "ours"​, ​"ourselves"​, ​"you",​ ​"your"​, ​"yours"​, ​"yourself"​, ​"yourselves",​ ​"he"​, "him"​, ​"his"​, ​"himself"​, ​"she",​ ​"her"​, ​"hers"​, ​"herself"​, ​"it",​ ​"its"​, ​"itself"​, "they"​, ​"them"​, ​"their"​, ​"theirs"​, "​themselves"​, ​"what"​, ​"which"​, ​"who",​ ​"whom"​, "this"​.. };
std​::​string​​get_destination_path​(​const​​std​::​string​& output_text, file_compare func)
{
​std​::​string​path; ​while​(​true​)
{
​std​::​cout​<< output_text;
getline(​std​::​cin​, path);
​if​(​std​::filesystem::exists(path) && func(path)) ​break;​
​std​::​cout​<< ​"Entered a wrong path, please enter the path again!\n"​;
}
​return​path; }

This function is used to find the destination of the documents to be compared for content similarity.

const​​std​::​string​plagiarism_file_path{ get_destination_path(​"Enter path of plagiarism file : "​, ​std​::filesystem::is_regular_file) };
std​::ifstream plagiarism_file{ plagiarism_file_path }; if​(!plagiarism_file.is_open())
{
​std​::​cout​<< ​"Please enter a valid plagiarism file!\n"​;
​std​::​exit​(​1​); }

To open the plagiarism file.

​std​::​vector​<​std​::​string​> target_lines; ​std​::​string​line; ​while​(getline(plagiarism_file, line, ​'.'​)) {
​std​::​string​result; ​std​::​string​transform_result;
    ​std​::transform(line.begin(), line.end(), std​::back_inserter(transform_result),[](​auto​val)
{
​return​​std​::​tolower​(val);
});
​std​::unique_copy(transform_result.begin(), transform_result.end(), std​::back_inserter(result), [](​auto​val1, ​auto​val2)
{
​return​​std​::​isspace​(val1) && ​std​::​isspace​(val2);
});
result.erase(​std​::remove_if(result.begin(), result.end(), [](​auto​val) {
​return​​std​::​ispunct​(val);
}), result.end()); target_lines.emplace_back(result);
}

By investigating each character of the files we’re comparing, we aim to find if there is any space characters and if so, the program delete

 double​​accuracy​(​const​s​td​::s​et​<​std​::​string​>& cmp1, ​const​​std​::​set​<​std​::​string​>& cmp2)
{
​int​accuracy = ​0​;
​for​(​const​​auto​& item1 : cmp1) {
​for​(​const​​auto​& item2: cmp2) {
​if​(item1 == item2) {
++accuracy;
​break​; }
​return​(​static_cast​<​double​>(accuracy) / s​td​::max(cmp1.size(), cmp2.size())) * 100.​;
}
} }

Transforming of the destination texts.

 std​::​vector​<​std​::​vector​<​std​::​string>​> source_documents; for​(​auto​& item: document_files)
{
source_documents.emplace_back(​std​::​vector​<​std​::​string​>{}); ​while​(getline(item, line, ​'.'​))
{
​std​::​string​result;

   ​std​::​string​transform_result;
​std​::transform(line.begin(), line.end(), std​::back_inserter(transform_result), [](​auto​val)
{
​return​​std​::​tolower​(val);
});
​std​::unique_copy(transform_result.begin(), transform_result.end(), std​::back_inserter(result), [](​auto​val1, ​auto​val2)
{
​return​​std​::​isspace​(val1) && ​std​::​isspace​(val2);
});
result.erase(​std​::remove_if(result.begin(), result.end(), [](​auto​val) {
​return​​std​::​ispunct​(val);
}), result.end()); source_documents.back().emplace_back(result);
} }

This part of the code is to output the number of similarity rate after comparison of the files. <map> and <vector> library functions are used to access the items.
  
 int​cnt{};
​for​(​const​​auto​& item : most_similar_sentences_in_documents) {
​if​(cnt == ​0​){ ​std​::​cout​<<​"--------------------------------------------------"​;
}
​std​::​cout​<<​"\n"​;
​std​::​cout​<<​"Document #"​<< cnt++ << "​ Similarity Rate => "​<< item.first
<< ​"%\n"​;
​std​::​vector​<​std​::pair<​std​::​string​, ​double​>> vec;
​for​(​const​​auto​&map_item : item.second) {
        vec.emplace_back(map_item.first, map_item.second);
     }
​std​::sort(vec.begin(), vec.end(), [](​auto​val, ​auto​val2) {
​return​val.second > val2.second; });
​std​::for_each_n(vec.begin(), ​5​, [](​auto​val) {
​std​::​cout​<< ​"-"​<< val.first << ​" => [RATE] : "​<< val.second << ​"%\n"​; });
​std​::​cout​<<​"\n"​; ​std​::​cout​<<​"--------------------------------------------------"​; ​std​::​cout​<<​"\n"​;
} }
 
  This part is to calculate the duration of execute time of the program.
  
 auto​stop_timer = ​std​::chrono::high_resolution_clock::now();
auto​duration = ​std​::chrono::duration_cast<​std​::chrono::milliseconds>(stop_timer - start_timer);
std​::​cout​<< ​std​::​endl​;
std​::​cout​<< ​"Finished within "​<< duration.count() << ​" milliseconds "​<< std​::​endl​;
  
Which libraries did we use?
  
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> //C++17 #include <vector>
#include <cstdlib> #include <map> #include <utility> #include <set> #include <algorithm> #include <chrono>
  
What is the Big-Oh Complexity?
  
It’s n^2 and in detail Big-Oh of functions: get_an_exist_path O(1)
tokenize O(n)
accuracy O (n^2)
main O ((n^2) * m)
  
What is the average execution time for the project?
  
50 milliseconds.
  
Which methods did we use?
  
We used binary search from map and from the vector library, we used string matching method

  Output:
  
 C:\Users\axsd\CLionProjects\untitled1\cmake-build-debug\untitled1. exe
Enter a ​set​of documents path :C:\Users\axsd\CLionProjects\untitled1\document-​set
 Enter plagiarism file path
:C:\Users\axsd\CLionProjects\untitled1\english_doc.txt
Finished within ​50 ​milliseconds --------------------------------------------------
  
Document #​0​Similarity Rate => ​100​%
- at the same time business correspondence syntactical pattern style is drawn to reach agreement between them => [RATE]
: ​100​%
- considering that rights ​and​duties must be validated by means of documents => [RATE] : ​100​%
- english documents are characterized from both legal ​and linguistic points of view => [RATE] : ​100​%
- in view of ​this​judicial documents should be drafted correctly => [RATE] : ​100​%
- now that we organized special courses teaching legal writing in this​work the authors pay attention to business docume
nts such as contracts legal correspondence claims etc => [RATE] : 100​%
--------------------------------------------------
Document #​1​Similarity Rate => ​3.25203​%
- considering that rights ​and​duties must be validated by means of documents => [RATE] : ​18.1818​%
- english documents are characterized from both legal ​and linguistic points of view => [RATE] : ​18.1818​%
- in view of ​this​judicial documents should be drafted correctly => [RATE] : ​18.1818​%
- the work is aimed at analyzing linguistic peculiarities of drafting legal documents => [RATE] : ​18.1818​%
- we truly believe that teaching documents writing now at university is a very important issue ​if​we want to bring up co mprehendible generation of lawyers => [RATE] : ​14.2857​%
--------------------------------------------------
 
Document #​2​Similarity Rate => ​4.06504​%
- in view of ​this​judicial documents should be drafted correctly => [RATE] : ​28.5714​%
- considering that rights ​and​duties must be validated by means of documents => [RATE] : ​25​%
- english documents are characterized from both legal ​and linguistic points of view => [RATE] : ​25​%
- the work is aimed at analyzing linguistic peculiarities of drafting legal documents => [RATE] : ​22.2222​%
- some years ago we were ​not​teaching legal writing => [RATE] : 16.6667​%
-------------------------------------------------- Process finished with ​exit​code ​0
