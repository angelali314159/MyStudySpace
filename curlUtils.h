#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <exception>
#include "json.h"
#include <curl/curl.h>

using namespace std;
using json = nlohmann::json;

class CurlException : public exception {
private:
    string errorMessage;
public:
    CurlException(const string& message) : errorMessage(message) {}
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

class SpotifyException : public exception {
private:
    string message;
public:
    SpotifyException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string Replace(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}


json SpotifyCurlInternal(const string& request, const string& endpoint, const map<string, string>& options, const string& authToken, const string& body = "") {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw CurlException("Failed to initialize cURL");
    }

    string url = "https://api.spotify.com" + endpoint;
    if (!options.empty()) {
        url += "?";
        for (const auto& option : options) {
            url += option.first + "=" + Replace(option.second, " ", "%20") + '&';
        }
        url.pop_back(); // Remove the last '&' character
    }

    curl_slist* headers = nullptr;
    string authHeader = "Authorization: Bearer " + authToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    string readBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if (!body.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    }

    // debugger
    cout << "Request URL: " << url << endl;


    CURLcode rc = curl_easy_perform(curl);
    if (rc != CURLE_OK) {
        string errorMessage = curl_easy_strerror(rc);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        throw CurlException(errorMessage);
    }

    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

    // debugger
    if (statusCode < 200 || statusCode > 299) {
        cout << "Response body: " << readBuffer << endl;
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        if (!readBuffer.empty()) {
            // Attempt to parse the error message from Spotify
            try {
                json errorJson = json::parse(readBuffer);
                string errorMsg = errorJson.value("error", "Unknown error occurred");
                throw SpotifyException("HTTP Error: Status code " + to_string(statusCode) + " - " + errorMsg);
            } catch (const json::exception& e) {
                // Fallback in case parsing fails
                throw SpotifyException("HTTP Error: Status code " + to_string(statusCode) + " - Failed to parse error response");
            }
        } else {
            // No additional information available
            throw SpotifyException("HTTP Error: Status code " + to_string(statusCode));
        }
    }


    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    try {
        return json::parse(readBuffer);
    } catch (json::parse_error& e) {
        throw SpotifyException("JSON Parse Error: " + string(e.what()));
    }
}

// spotify api requests

// SPOTIFY GET //
json SpotifyGET(string endpoint, map<string, string> options, string authToken, string body = ""){
    return SpotifyCurlInternal("GET", endpoint, options, authToken, body);
}

// SPOTIFY POST//
json SpotifyPOST(string endpoint, map<string, string> options, string authToken, string body = ""){
    return SpotifyCurlInternal("POST", endpoint, options, authToken, body);
}

// SPOTIFY PUT //
json SpotifyPUT(string endpoint, map<string, string> options, string authToken, string body = "")
{
    return SpotifyCurlInternal("PUT", endpoint, options, authToken, body);
}

// SPOTIFY DELETE //
json SpotifyDELETE(string endpoint, map<string, string> options, string authToken, string body = ""){
    return SpotifyCurlInternal("DELETE", endpoint, options, authToken, body);
}
