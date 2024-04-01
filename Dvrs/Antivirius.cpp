#include <iostream>
#include <string>
#include <unordered_set> // Include unordered_set to store blocked extensions efficiently

// Function to simulate downloading a file
void downloadFile(const std::string& fileName) {
    // Simulate downloading the file
    std::cout << "Downloading file: " << fileName << std::endl;
    // Implementation of downloading logic goes here...
}

// Function to prompt the user whether to allow or block a file extension
bool promptUser(const std::string& fileExtension) {
    // Ask the user whether to allow or block the file extension
    std::cout << "Do you want to allow downloading files with the extension " << fileExtension << "? (Y/N): ";
    char response;
    std::cin >> response;
    return (response == 'Y' || response == 'y');
}

// Function to check if the file extension should be blocked
bool shouldBlockFile(const std::string& fileName, std::unordered_set<std::string>& blockedExtensions) {
    // Get the file extension by finding the last '.' in the file name
    size_t lastDotPos = fileName.find_last_of(".");
    if (lastDotPos != std::string::npos) {
        std::string fileExtension = fileName.substr(lastDotPos);

        // Check if the file extension is in the set of blocked extensions
        return blockedExtensions.find(fileExtension) != blockedExtensions.end();
    }

    // If no extension is found, consider the file safe
    return false;
}

// Function to initiate a file download
void initiateDownload(const std::string& fileName, std::unordered_set<std::string>& blockedExtensions) {
    if (shouldBlockFile(fileName, blockedExtensions)) {
        std::cout << "Download of file " << fileName << " blocked due to file extension." << std::endl;
    } else {
        downloadFile(fileName);
    }
}

int main() {
    // Example file names to download
    std::string file1 = "document.pdf";
    std::string file2 = "data.aaa";
    std::string file3 = "report.doc";
    std::string file4 = "image.jpg";
    std::string file5 = "info.locked";
    std::string file6 = "setup.exe";
    std::string file7 = "test.exe";

    // List of blocked file extensions
    std::unordered_set<std::string> blockedExtensions;

    // Ask the user whether to allow or block each file extension
    for (const std::string& fileName : {file1, file2, file3, file4, file5, file6, file7}) {
        size_t lastDotPos = fileName.find_last_of(".");
        if (lastDotPos != std::string::npos) {
            std::string fileExtension = fileName.substr(lastDotPos);
            if (!blockedExtensions.count(fileExtension)) {
                if (!promptUser(fileExtension)) {
                    blockedExtensions.insert(fileExtension);
                }
            }
        }
    }

    // Initiate download for each file
    initiateDownload(file1, blockedExtensions);
    initiateDownload(file2, blockedExtensions);
    initiateDownload(file3, blockedExtensions);
    initiateDownload(file4, blockedExtensions);
    initiateDownload(file5, blockedExtensions);
    initiateDownload(file6, blockedExtensions);
    initiateDownload(file7, blockedExtensions);

    return 0;
}
