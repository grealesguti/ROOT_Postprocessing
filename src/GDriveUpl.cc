#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

// Callback function to write data to the file
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* file = static_cast<std::ofstream*>(userp);
    file->write(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

// Upload a file to a Google Drive folder
void uploadFileToGoogleDrive(const std::string& filePath, const std::string& folderId, const std::string& accessToken) {
    std::ifstream fileStream(filePath, std::ios::binary);
    if (!fileStream) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Get file size
    fileStream.seekg(0, std::ios::end);
    std::streampos fileSize = fileStream.tellg();
    fileStream.seekg(0, std::ios::beg);

    // Prepare CURL request
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set upload URL
        std::string url = "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart";
        if (!folderId.empty()) {
            url += "&parents=" + folderId;
        }

        // Set request headers
        struct curl_slist* headers = nullptr;
        std::string authorizationHeader = "Authorization: Bearer " + accessToken;
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        headers = curl_slist_append(headers, authorizationHeader.c_str());

        // Set file data to upload
        std::ofstream outputFileStream("output.txt"); // Temporary file to store server response
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_READDATA, &fileStream);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_PUT, 1L);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, fileSize);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputFileStream);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        fileStream.close();
        outputFileStream.close();

        if (res == CURLE_OK) {
            std::cout << "File uploaded successfully." << std::endl;
            remove("output.txt"); // Delete temporary response file
        } else {
            std::cerr << "File upload failed. Error: " << curl_easy_strerror(res) << std::endl;
        }
    } else {
        std::cerr << "Failed to initialize CURL." << std::endl;
    }
}

/*
  To obtain the accessToken and folderId required for the uploadFileToGoogleDrive function, you need to go through the following steps:

Set up a Google Cloud Platform (GCP) project:

Go to the Google Cloud Console and create a new project.
Enable the Google Drive API for your project.
Create credentials (OAuth 2.0 client ID) for your project.
Authenticate and authorize the application:

Use the client ID and client secret obtained from the credentials to authenticate your application and request authorization from the user to access their Google Drive.
Follow the appropriate authentication flow (web, desktop, or server) to obtain an authorization code.
Exchange the authorization code for an access token and refresh token:

Make a POST request to the token endpoint with the authorization code, client ID, client secret, and redirect URI to exchange it for an access token and refresh token.
The response will include the accessToken needed for the uploadFileToGoogleDrive function.
Get the folder ID:

To upload a file to a specific folder, you need to obtain the folderId of the target folder.
You can use the Google Drive API to list the folders in the user's Drive or search for a specific folder by name.
Once you have the folder, you can extract its folderId to use in the uploadFileToGoogleDrive function.
Note that the exact steps and code required for authentication, authorization, and obtaining the folder ID can be complex and depend on the specific implementation and the platform you are developing for (web, desktop, or server). It involves making HTTP requests, handling OAuth 2.0 flows, and working with APIs.

You may want to explore the official Google Drive API documentation and sample code for your chosen platform or programming language for more detailed instructions and examples.
 */
