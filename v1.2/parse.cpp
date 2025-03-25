#include <bits/stdc++.h>
#ifdef _WIN32
    #include <direct.h>   // for _mkdir
#else
    #include <sys/stat.h> // for mkdir
#endif
#include <errno.h>
#include <curl/curl.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

// Global variable for username (filled at runtime)
string globalUsername;

// Structure to store problem data
struct Problem {
    string id;
    string name;
    string url;
    vector<pair<string, string>> testCases; // each pair: {input, expected output}
};

// Structure for contest
struct Contest {
    string platform;
    string id;
    string name;
    vector<Problem> problems;
};

// Callback function for CURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch(std::bad_alloc& e) {
        return 0;
    }
}

// Function to perform login for AtCoder
bool performAtCoderLogin(const string& username, const string& password) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize CURL" << endl;
        return false;
    }

    string loginUrl = "https://atcoder.jp/login";
    string postFields = "username=" + username + "&password=" + password;
    string readBuffer;

    curl_easy_setopt(curl, CURLOPT_URL, loginUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");  // Save cookies
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt"); // Read cookies
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // Check if login succeeded by looking for "Sign Out" in the response
    if (readBuffer.find("Sign Out") != string::npos) {
        cout << "AtCoder login successful!" << endl;
    } else {
        cerr << "AtCoder login failed! Please check your credentials." << endl;
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}

// Function to fetch web content using CURL (with session for AtCoder)
string fetchWebContent(const string& url, bool useSession = false) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize CURL" << endl;
        return "";
    }

    string readBuffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (useSession) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt"); // Use session cookie for AtCoder
    }
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    }

    curl_easy_cleanup(curl);
    return readBuffer;
}

// Function to create directory if it doesn't exist
bool createDirectory(const string& path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
#else
    return mkdir(path.c_str(), 0777) == 0 || errno == EEXIST;
#endif
}

// Function to write a file
void writeToFile(const string& filename, const string& content) {
    ofstream file(filename, ios::out | ios::binary);
    if (file.is_open()) {
        file << content;
        file.close();
        cout << "Created file: " << filename << endl;
    } else {
        cerr << "Error creating file: " << filename << endl;
    }
}

// Generate C++ template using an embedded string literal (unchanged as per request)
string generateCppTemplate(const Problem& problem) {
    const string templateContent = R"(/*
ID: ${USERNAME}
TASK: ${TASK}
LANG: ${LANGUAGE}
*/

#ifdef LOCAL_DEBUG
#include <windows.h>
#include <psapi.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;
static chrono::steady_clock::time_point debug_start;

// Helper function to trim whitespace
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

string centeredHeader(const string& text, int width = 60) {
    int pad = (width - (int)text.size()) / 2;
    if (pad < 0) pad = 0;
    return string(pad, '-') + " " + text + " " + string(pad, '-');
}

struct Debugger {
    Debugger() { debug_start = chrono::steady_clock::now(); }
    ~Debugger() {
        static istream* pIn;
        if (pIn == &cin) return;

        auto end = chrono::steady_clock::now();
        long long time_taken = chrono::duration_cast<chrono::milliseconds>(end - debug_start).count();

        ifstream actualFile("test.out");
        vector<string> actualLines;
        string line;
        while (getline(actualFile, line)) {
            string t = trim(line);
            if (!t.empty()) actualLines.push_back(t);
        }

        ifstream expectedFile("test-expected.out");
        vector<string> expectedLines;
        while (getline(expectedFile, line)) {
            string t = trim(line);
            if (!t.empty()) expectedLines.push_back(t);
        }

        cerr << "\n" << string(60, '=') << "\n";
        cerr << centeredHeader("DEBUG SUMMARY") << "\n";
        cerr << string(60, '=') << "\n\n";

        cerr << centeredHeader("OUTPUT") << "\n";
        for (auto &l : actualLines) cerr << "  " << l << "\n";
        if (actualLines.empty()) cerr << "  (no output)\n";
        cerr << "\n";

        cerr << centeredHeader("EXPECTED OUTPUT") << "\n";
        for (auto &l : expectedLines) cerr << "  " << l << "\n";
        if (expectedLines.empty()) cerr << "  (none provided)\n";
        cerr << "\n";

        cerr << centeredHeader("COMPARISON") << "\n";
        int total = (int)expectedLines.size(), passed = 0;
        for (int i = 0; i < total; i++) {
            if (i < (int)actualLines.size() && expectedLines[i] == actualLines[i]) passed++;
        }
        if (total == 0) cerr << "  No expected output provided.\n";
        else if (passed == total) cerr << " All test cases passed (" << passed << "/" << total << ")\n";
        else cerr << (total - passed) << " test case(s) failed (" << passed << "/" << total << ")\n";
        cerr << "\n";

        cerr << centeredHeader("PERFORMANCE") << "\n";
        cerr << "  Time taken:  " << time_taken << " ms\n";
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
            cerr << "  Memory used: " << (pmc.WorkingSetSize / 1024) << " KB\n";
        else
            cerr << "  Memory used: N/A\n";
        cerr << string(60, '=') << "\n";
    }
};
static Debugger _debugger;
#endif

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <numeric>
#include <stdint.h>
#include <cmath>
#include <climits>
#include <utility>
#include <chrono>
#include <functional>
#ifdef __linux__
#include <sys/resource.h>
#endif

using namespace std;

#define int64 int64_t
#define int32 int32_t
#define ll long long

static istream* pIn = &cin;
static ostream* pOut = &cout;

#define cin (*pIn)
#define cout (*pOut)

void setupIO() {
    static ifstream fin("test.in");
    if (fin.is_open()) {
        static ofstream fout("test.out");
        if (fout.is_open()) {
            pIn = &fin;
            pOut = &fout;
        }
    }
}

// --------------------
// Start Solution Section
// --------------------

int T = ${T};

void solve() {
    // code here
}

int32 main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    setupIO();

    // cin >> T;
    while (T--) solve();

    return 0;
}
)";

    // Replace placeholders with actual values
    string finalTemplate = regex_replace(templateContent, regex("\\$\\{USERNAME\\}"), globalUsername);
    finalTemplate = regex_replace(finalTemplate, regex("\\$\\{TASK\\}"), problem.name);
    finalTemplate = regex_replace(finalTemplate, regex("\\$\\{LANGUAGE\\}"), "C++17");
    finalTemplate = regex_replace(finalTemplate, regex("\\$\\{T\\}"), to_string(problem.testCases.size()));
    
    return finalTemplate;
}

// Create all files for a problem
void createProblemFiles(const string& contestDir, const Problem& problem) {
    string problemDir = contestDir + "/Problem_" + problem.id;
    if (!createDirectory(problemDir)) {
        cerr << "Failed to create directory: " << problemDir << endl;
        return;
    }
    // Create solution.cpp file using the template
    writeToFile(problemDir + "/solution.cpp", generateCppTemplate(problem));
    
    // Merge each test case's input and expected output into separate files.
    ostringstream mergedInput, mergedExpected;
    for (size_t i = 0; i < problem.testCases.size(); ++i) {
        // Write each test case's input block preserving its newlines.
        mergedInput << problem.testCases[i].first << "\n";
        mergedExpected << problem.testCases[i].second << "\n";
    }
    
    writeToFile(problemDir + "/test.in", mergedInput.str());
    writeToFile(problemDir + "/test-expected.out", mergedExpected.str());
    
    // test.out will be generated when your solution runs in debug mode.
}

// Helper: Process a raw sample block that contains multiple test-example-line divs.
string processTestExampleBlock(const string &raw) {
    string result;
    regex lineRegex(R"(<div class="test-example-line[^>]*>(.*?)<\/div>)");
    smatch m;
    string::const_iterator searchStart(raw.cbegin());
    while (regex_search(searchStart, raw.cend(), m, lineRegex)) {
        // Append the inner text and a newline.
        result += m[1].str() + "\n";
        searchStart = m.suffix().first;
    }
    return result;
}

// Function to parse Codeforces contest with updated sample test extraction
Contest parseCodeforcesContest(const string& contestId) {
    Contest contest;
    contest.platform = "Codeforces";
    contest.id = contestId;
    
    string contestUrl = "https://codeforces.com/contest/" + contestId;
    string contestPage = fetchWebContent(contestUrl);
    
    // Extract contest name from <title>
    size_t namePos = contestPage.find("<title>") + 7;
    size_t nameEnd = contestPage.find(" - ", namePos);
    if (namePos != string::npos && nameEnd != string::npos) {
        contest.name = contestPage.substr(namePos, nameEnd - namePos);
    } else {
        contest.name = "Contest " + contestId;
    }
    
    // Find problems using the common URL pattern
    string problemPattern = "<a href=\"/contest/" + contestId + "/problem/";
    size_t problemPos = contestPage.find(problemPattern);
    map<string, bool> addedProblems;
    
    while (problemPos != string::npos) {
        size_t idStart = problemPos + problemPattern.length();
        size_t idEnd = contestPage.find("\"", idStart);
        if (idStart != string::npos && idEnd != string::npos) {
            string problemId = contestPage.substr(idStart, idEnd - idStart);
            if (!addedProblems[problemId]) {
                addedProblems[problemId] = true;
                Problem problem;
                problem.id = problemId;
                problem.url = contestUrl + "/problem/" + problemId;
                
                // Fetch problem page
                string problemPage = fetchWebContent(problem.url);
                
                // Extract problem title from <div class="title">
                string titlePattern = "<div class=\"title\">";
                size_t titlePos = problemPage.find(titlePattern);
                if (titlePos != string::npos) {
                    titlePos += titlePattern.length();
                    size_t titleEnd = problemPage.find("</div>", titlePos);
                    if (titleEnd != string::npos) {
                        size_t dotPos = problemPage.find(".", titlePos);
                        if (dotPos != string::npos && dotPos < titleEnd) {
                            problem.name = problemPage.substr(dotPos + 1, titleEnd - dotPos - 1);
                            problem.name = problem.name.substr(problem.name.find_first_not_of(" \t\n\r"));
                        } else {
                            problem.name = "Problem " + problemId;
                        }
                    }
                }
                
                // Extract sample test cases.
                // The input/output blocks might either be in a single <pre> or split into multiple divs.
                string inputPattern = "<div class=\"input\"><div class=\"title\">Input</div><pre>";
                string outputPattern = "<div class=\"output\"><div class=\"title\">Output</div><pre>";
                size_t inputPos = problemPage.find(inputPattern);
                while (inputPos != string::npos) {
                    inputPos += inputPattern.length();
                    size_t inputEnd = problemPage.find("</pre>", inputPos);
                    if (inputEnd == string::npos)
                        break;
                    
                    // Extract the raw input block.
                    string rawInput = problemPage.substr(inputPos, inputEnd - inputPos);
                    string input;
                    // If the raw block contains multiple divs, process them.
                    if (rawInput.find("test-example-line") != string::npos) {
                        input = processTestExampleBlock(rawInput);
                    } else {
                        // Otherwise, use a simple cleanup.
                        input = rawInput;
                        input = regex_replace(input, regex("\r\n"), "\n");
                        input = regex_replace(input, regex("\r"), "\n");
                    }
                    
                    // Now extract the corresponding output block.
                    size_t outputPos = problemPage.find(outputPattern, inputEnd);
                    if (outputPos == string::npos)
                        break;
                    outputPos += outputPattern.length();
                    size_t outputEnd = problemPage.find("</pre>", outputPos);
                    if (outputEnd == string::npos)
                        break;
                    
                    string rawOutput = problemPage.substr(outputPos, outputEnd - outputPos);
                    string output;
                    if (rawOutput.find("test-example-line") != string::npos) {
                        output = processTestExampleBlock(rawOutput);
                    } else {
                        output = rawOutput;
                        output = regex_replace(output, regex("\r\n"), "\n");
                        output = regex_replace(output, regex("\r"), "\n");
                    }
                    
                    // (Optionally, you can apply further HTML entity decoding if needed.)
                    
                    // Save the cleaned test case.
                    problem.testCases.push_back({input, output});
                    
                    // Search for the next input block after the current output block.
                    inputPos = problemPage.find(inputPattern, outputEnd);
                }
                
                if (!problem.testCases.empty()) {
                    contest.problems.push_back(problem);
                    cout << "Found problem " << problemId << ": " << problem.name 
                         << " with " << problem.testCases.size() << " test case(s)" << endl;
                }
            }
        }
        problemPos = contestPage.find(problemPattern, problemPos + 1);
    }
    
    return contest;
}

// Function to parse AtCoder contest using the tasks page
Contest parseAtCoderContest(const string& contestId, bool useSession) {
    Contest contest;
    contest.platform = "AtCoder";
    contest.id = contestId;
    
    string contestUrl = "https://atcoder.jp/contests/" + contestId;
    // Use tasks page to get problem list, since the main contest page may hide details during an ongoing contest
    string tasksUrl = contestUrl + "/tasks";
    string tasksPage = fetchWebContent(tasksUrl, useSession);
    
    // Get contest name from contest homepage if available
    string contestPage = fetchWebContent(contestUrl, useSession);
    size_t namePos = contestPage.find("<title>") + 7;
    size_t nameEnd = contestPage.find(" - ", namePos);
    if (namePos != string::npos && nameEnd != string::npos) {
        contest.name = contestPage.substr(namePos, nameEnd - namePos);
    } else {
        contest.name = "Contest " + contestId;
    }
    
    // Parse tasksPage for problems
    string problemPattern = "href=\"/contests/" + contestId + "/tasks/";
    size_t problemPos = tasksPage.find(problemPattern);
    map<string, bool> addedProblems;
    
    while (problemPos != string::npos) {
        size_t idStart = problemPos + problemPattern.length();
        size_t idEnd = tasksPage.find("\"", idStart);
        if (idStart != string::npos && idEnd != string::npos) {
            string fullProblemId = tasksPage.substr(idStart, idEnd - idStart);
            string problemId = fullProblemId;
            size_t underscorePos = fullProblemId.find_last_of("_");
            if (underscorePos != string::npos) {
                problemId = fullProblemId.substr(underscorePos + 1);
            }
            if (!addedProblems[fullProblemId]) {
                addedProblems[fullProblemId] = true;
                Problem problem;
                problem.id = problemId;
                problem.url = contestUrl + "/tasks/" + fullProblemId;
                
                // Get problem name from tasksPage
                size_t nameStart = tasksPage.find(">", idEnd) + 1;
                size_t nameEnd = tasksPage.find("</a>", nameStart);
                if (nameStart != string::npos && nameEnd != string::npos) {
                    problem.name = tasksPage.substr(nameStart, nameEnd - nameStart);
                } else {
                    problem.name = "Problem " + problemId;
                }
                
                // Attempt to fetch sample tests from the problem page
                string problemPage = fetchWebContent(problem.url, useSession);
                string samplePattern = "<h3>Sample Input";
                int testCaseNumber = 1;
                size_t samplePos = problemPage.find(samplePattern);
                while (samplePos != string::npos) {
                    string inputPattern = "<pre>";
                    size_t inputPos = problemPage.find(inputPattern, samplePos);
                    if (inputPos != string::npos) {
                        inputPos += inputPattern.length();
                        size_t inputEnd = problemPage.find("</pre>", inputPos);
                        if (inputEnd != string::npos) {
                            string input = problemPage.substr(inputPos, inputEnd - inputPos);
                            string outputPattern = "<h3>Sample Output " + to_string(testCaseNumber) + "</h3>";
                            size_t outputHeaderPos = problemPage.find(outputPattern, inputEnd);
                            if (outputHeaderPos != string::npos) {
                                size_t outputPos = problemPage.find("<pre>", outputHeaderPos);
                                if (outputPos != string::npos) {
                                    outputPos += 5;
                                    size_t outputEnd = problemPage.find("</pre>", outputPos);
                                    if (outputEnd != string::npos) {
                                        string output = problemPage.substr(outputPos, outputEnd - outputPos);
                                        auto cleanHtml = [](string& s) {
                                            regex r("&lt;"); s = regex_replace(s, r, "<");
                                            r = regex("&gt;"); s = regex_replace(s, r, ">");
                                            r = regex("&quot;"); s = regex_replace(s, r, "\"");
                                            r = regex("&amp;"); s = regex_replace(s, r, "&");
                                        };
                                        cleanHtml(input);
                                        cleanHtml(output);
                                        problem.testCases.push_back({input, output});
                                        testCaseNumber++;
                                        samplePos = problemPage.find("<h3>Sample Input " + to_string(testCaseNumber) + "</h3>", outputEnd);
                                    } else {
                                        break;
                                    }
                                } else {
                                    break;
                                }
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                // For ongoing contests sample tests might be missing; still add the problem
                contest.problems.push_back(problem);
                cout << "Found problem " << problemId << ": " << problem.name 
                     << " with " << problem.testCases.size() << " test case(s)" << endl;
            }
        }
        problemPos = tasksPage.find(problemPattern, problemPos + 1);
    }
    
    return contest;
}

// Main function with modified platform input
int main() {
    cout << "Online Contest Problem Parser" << endl;
    cout << "============================" << endl;
    cout << "Supported platforms: Codeforces, AtCoder" << endl << endl;
    
    cout << "Enter your username: ";
    getline(cin, globalUsername);
    
    char platformChar;
    string contestId;
    cout << "Enter platform (a for AtCoder, c for Codeforces): ";
    cin >> platformChar;
    
    cout << "Enter contest ID: ";
    cin >> contestId;
    
    platformChar = tolower(platformChar);
    
    bool useSession = false;
    if (platformChar == 'a') {
        // For AtCoder, attempt to login if credentials are provided
        const char* atcoderPassword = getenv("ATCODER_PASSWORD");
        if (atcoderPassword && performAtCoderLogin(globalUsername, atcoderPassword)) {
            useSession = true;
        } else {
            cout << "Proceeding without login for AtCoder. Some features may be limited for ongoing contests." << endl;
        }
    }
    
    Contest contest;
    if (platformChar == 'c') {
        cout << "Fetching Codeforces contest " << contestId << "..." << endl;
        contest = parseCodeforcesContest(contestId);
    } else if (platformChar == 'a') {
        cout << "Fetching AtCoder contest " << contestId << "..." << endl;
        contest = parseAtCoderContest(contestId, useSession);
    } else {
        cerr << "Unsupported platform. Use 'c' for Codeforces or 'a' for AtCoder." << endl;
        return 1;
    }
    
    if (contest.problems.empty()) {
        cerr << "No problems found in the contest." << endl;
        return 1;
    }
    
    string contestDir = contest.platform + "_" + contest.id;
    if (!createDirectory(contestDir)) {
        cerr << "Failed to create contest directory!" << endl;
        return 1;
    }
    
    stringstream readmeContent;
    readmeContent << "# " << contest.name << endl << endl;
    readmeContent << "Platform: " << contest.platform << endl;
    readmeContent << "Contest ID: " << contest.id << endl;
    readmeContent << "URL: ";
    
    if (contest.platform == "Codeforces") {
        readmeContent << "https://codeforces.com/contest/" << contest.id << endl;
    } else if (contest.platform == "AtCoder") {
        readmeContent << "https://atcoder.jp/contests/" << contest.id << endl;
    }
    
    readmeContent << endl << "## Problems" << endl << endl;
    for (const Problem& problem : contest.problems) {
        readmeContent << "### Problem " << problem.id << ": " << problem.name << endl;
        readmeContent << "- URL: " << problem.url << endl;
        readmeContent << "- Test Cases: " << problem.testCases.size() << endl << endl;
    }
    
    writeToFile(contestDir + "/README.md", readmeContent.str());
    
    for (const Problem& problem : contest.problems) {
        createProblemFiles(contestDir, problem);
    }
    
    cout << endl << "Contest parsed successfully!" << endl;
    cout << "Created directory: " << contestDir << endl;
    cout << "Found " << contest.problems.size() << " problems." << endl;
    
    return 0;
}
