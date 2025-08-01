#pragma once
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;

class PKCEUtils
{
public:
    static QString generateCodeVerifier()
    {
        QString codeVerifier;

        const int length = 43 + QRandomGenerator::global()->bounded(86);

        for(int i = 0; i < length; i++)
        {
            const auto index = QRandomGenerator::global()->bounded(unreservedChars.length());
            codeVerifier.append(unreservedChars.at(index));
        }

        return codeVerifier;
    }

    static QString generateCodeChallenge(const QString& codeVerifier)
    {
        const auto hash = QCryptographicHash::hash(codeVerifier.toLatin1(), QCryptographicHash::Sha256);

        return hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
    }

    static QString generateState()
    {
        QString state;

        const int length = 12 + QRandomGenerator::global()->bounded(24);

        for(int i = 0; i < length; i++)
        {
            const auto index = QRandomGenerator::global()->bounded(unreservedChars.length());
            state.append(unreservedChars.at(index));
        }

        return state;
    }

private:
    inline static const auto unreservedChars = u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~"_s;
};
