#include <iostream>
#include <fstream>  
#include <charconv>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

std::mutex mutexLock;

// Begin of functions.
bool CheckForUniqueWord(vector<string> VectorUniqueWords, char WordForWrite[]);
string SearchEcnryption(vector<string> VectorWords, vector<string> VectorWords2, char WordForWrite[]);
void ReadWordFromFile(ifstream& ObjectReadFile, char* ArrayForRewrite);
void ProcesingDoc(string FileName, vector<string>& VectorEcnrWords, vector<string>& VectorUniqueWords);
void EncryptWord(char* arrayForEncrypt);
string GetFileNameFromPath(string FilePath);
// End of functions.

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    srand(static_cast<unsigned int>(time(0)));

    // Doc names.
    string Doc1FileName;
    string Doc2FileName;

    vector<string> VectorUniqueWords;    // Vector for uninque words.
    vector<string> VectorEncryptedWords; // Vector for encrytped words.

    // If only file name entered, file will be selected from project folder.
    // Note, that you should enter different slash '/', not a '\' like in explorer file path.
    // Created files will be saved in project folder.
    cout << "Enter path and name of documents. Example: C:/Games/Doc1.txt" << endl << "First document: ";
    cin >> Doc1FileName;
    cout << "Second document: ";
    cin >> Doc2FileName;

    // Start threads.
    thread thread1(ProcesingDoc, Doc1FileName, std::ref(VectorEncryptedWords), std::ref(VectorUniqueWords));
    thread thread2(ProcesingDoc, Doc2FileName, std::ref(VectorEncryptedWords), std::ref(VectorUniqueWords));
    // End of threads.

    thread1.join();
    thread2.join();

    /*
    for (int i = 0, x = VectorUniqueWords.size(); i < x; i++) {
        cout << "Word/Ecnryption: \t"  << VectorUniqueWords[i] << "\t - " << VectorEncryptedWords[i] << endl;
    }
    */
    cout << endl;
    system("pause");
    return 0;
}

/// <summary>
/// Receiving file name from file path.
/// </summary>
/// <param name="FilePath">File path.</param>
/// <returns>File name.</returns>
string GetFileNameFromPath(string FilePath)
{
    string fileName(20, '\0');
    size_t pathLen = FilePath.length();
    for (int i = 0; i < pathLen; i++) { // Reading string untill we meet '/'
        if (FilePath[i] != '/') {
            int j = 0;
            for (; i < pathLen && FilePath[i] != '/'; i++, j++) {

                fileName[j] = FilePath[i];
            }
            fileName[j++] = '\0';
        } else {
            fileName = " ";
        }
    }
    return fileName;
}

/// <summary>
/// Function checks uniqueness of word.
/// </summary>
/// <param name="VectorUniqueWords">Vector of uninque words.</param>
/// <param name="WordForWrite">The word for checking.</param>
/// <returns>Returns true if unique.</returns>
bool CheckForUniqueWord(vector<string> VectorUniqueWords, char WordForWrite[])
{
    int vectorSize = VectorUniqueWords.size();
    bool checkForUnique = true;
    for (int currentIdElement = 0; currentIdElement < vectorSize; currentIdElement++) {
        if (VectorUniqueWords[currentIdElement] == WordForWrite) {
            checkForUnique = false;
            break;
        }
    }
    return checkForUnique;
}

/// <summary>
/// Function searches and return encrytped version of transffered argument.
/// </summary>
/// <param name="VectorUniqueWords">Vector of uninque words.</param>
/// <param name="VectorEncryptedWords">Vector of encrypted words.</param>
/// <param name="WordForWrite">The word to be replaced.</param>
/// <returns>Returns ecnrypted word.</returns>
string SearchEcnryption(vector<string> VectorUniqueWords, vector<string> VectorEncryptedWords, char WordForWrite[])
{
    int vectorSize = VectorEncryptedWords.size();   // id of unique word in vector1 will match with id of his ecnrypted version in vector2
    for (int CurrentElement = 0; CurrentElement < vectorSize; CurrentElement++) {
        if (VectorUniqueWords[CurrentElement] == WordForWrite) { // searching id of unecnrypted word
            return VectorEncryptedWords[CurrentElement];    // return ecnrypted version of word
        }
    }
}

/// <summary>
/// Function reads the word from file by chars.
/// </summary>
/// <param name="ArrayForWrite">Array where word will be saved.</param>
/// <param name="ObjectReadFile">File where reading from.</param>
/// <returns>Nothing. Array is reffered.</returns>
void ReadWordFromFile(ifstream& ObjectReadFile, char* ArrayForWrite)
{
    int currentIdElement = 0;
    char charBuffer; // char buffer
    do {
        charBuffer = ObjectReadFile.get();
        ArrayForWrite[currentIdElement] = charBuffer;
        currentIdElement++;
        if (!(isdigit(charBuffer) ||
            isalpha(charBuffer))
            && currentIdElement != 1) {
            ArrayForWrite[currentIdElement - 1] = '\0';
            ObjectReadFile.seekg(-1, ios::cur);
            break;
        }
    } while ((isdigit(charBuffer) || isalpha(charBuffer)) && !ObjectReadFile.eof());
    ArrayForWrite[currentIdElement] = '\0';
}

/// <summary>
/// Function encrypt current word. Checks for digits, alphabet, register.
/// </summary>
/// <param name="arrayForEncrypt">A word for encryption.</param>
void EncryptWord(char* arrayForEncrypt)
{
    for (int currentIdSymbol = 0; arrayForEncrypt[currentIdSymbol]; currentIdSymbol++) {
        if (isdigit(arrayForEncrypt[currentIdSymbol])) {
            arrayForEncrypt[currentIdSymbol] = '0' + (rand() % 10);
        }
        else if (isalpha(arrayForEncrypt[currentIdSymbol])) {
            if (isupper(arrayForEncrypt[currentIdSymbol])) {
                arrayForEncrypt[currentIdSymbol] = char(toupper(char('a' + rand() % ('z' - 'a'))));
            }
            else {
                arrayForEncrypt[currentIdSymbol] = char(char('a' + rand() % ('z' - 'a')));
            }
        }
        else {
            arrayForEncrypt[currentIdSymbol] = arrayForEncrypt[currentIdSymbol];
        }
    }
}

/// <summary>
/// Function creates file objects and write encryption file.
/// </summary>
/// <param name="FileName">Name of file to be processed.</param>
/// <param name="VectorEcnrWords">Vector of encrypted words.</param>
/// <param name="VectorUniqueWords">Vector of unique words.</param>
void ProcesingDoc(string FileName, vector<string>& VectorEcnrWords, vector<string>& VectorUniqueWords)
{

    ifstream ObjectReadFile(FileName); // Object for docX (reading).

    if (!ObjectReadFile.is_open()) {
        cout << "File " << FileName << " cannot be opened or created.\n";
        return;
    }

    ofstream ObjectWriteFile("Random-" + GetFileNameFromPath(FileName), ios_base::out | ios_base::trunc); // Object for random-docX (writing).

    if (!ObjectWriteFile.is_open()) {
        cout << "File Random-" << FileName << " cannot be opened or created.\n";
        return;
    }

    char arrayForRewriteWord[50]; // Buffer char array for words.
    bool checkUnique = true;

    while (!ObjectReadFile.eof()) { // Cycle of rewriting docX

        ReadWordFromFile(ObjectReadFile, arrayForRewriteWord);

        // In function ReadWordFromFile there is Do-While 
        // and error occurs in the end of file, this IF prevents problem.
        if (ObjectReadFile.eof()) {
            break;
        }

        mutexLock.lock();

        checkUnique = CheckForUniqueWord(VectorUniqueWords, arrayForRewriteWord);

        if (checkUnique) {
            VectorUniqueWords.push_back(arrayForRewriteWord); // Saving unique word.
            EncryptWord(arrayForRewriteWord);
            VectorEcnrWords.push_back(arrayForRewriteWord); // Saving encrypted word.
        }
        else {
            strcpy_s(arrayForRewriteWord, SearchEcnryption(VectorUniqueWords, VectorEcnrWords, arrayForRewriteWord).c_str()); // Replacing word with encrypted.
        }

        mutexLock.unlock();

        ObjectWriteFile << arrayForRewriteWord;
    }
    ObjectWriteFile.close();
}