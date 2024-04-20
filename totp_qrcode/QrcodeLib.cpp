#include "pch.h"
#include "QrToPng.h"
#include "QrcodeLib.h"

#include <string>
#include <iostream>
#include <sstream>

std::string QrcodeLib::generateQrcode(std::string url) {
    std::string tempFilename = std::filesystem::temp_directory_path().u8string() + "tempfile_" + std::to_string(std::time(nullptr)) + ".png";

    int imgSize = 300;
    int minModulePixelSize = 3;

    auto qrPng = QrToPng(tempFilename, imgSize, minModulePixelSize, url, true, qrcodegen::QrCode::Ecc::MEDIUM);

    if (qrPng.writeToPNG())
        return tempFilename;
    else
        return NULL;
}

std::string QrcodeLib::createTotpUrl(std::string username, std::string secret) {
    // Encoding username for URL
    std::stringstream encodedUsername;
    for (char c : username) {
        if (isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~') {
            encodedUsername << c;
        }
        else {
            encodedUsername << '%' << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
        }
    }

    std::string url = "otpauth://totp/" + encodedUsername.str() + "?secret=" + secret + "&issuer=Wallet+System&algorithm=SHA1&digits=6&period=30";
    return url;
}