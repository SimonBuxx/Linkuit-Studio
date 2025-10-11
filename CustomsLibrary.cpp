/*!
 * \file CustomsLibrary.cpp
 * \brief The CustomsLibrary class implements a library for custom logic components
 * \author Simon Buchholz
 * \copyright Copyright (c) 2025, Simon Buchholz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "CustomsLibrary.h"

CustomsLibrary::CustomsLibrary(QObject *pParent)
    : QObject(pParent)
{
}

std::optional<std::shared_ptr<QJsonObject>> CustomsLibrary::GetCustomJson(const CircuitId& pCircuitId) const
{
    if (mCustoms.find(pCircuitId) == mCustoms.end())
    {
        return std::nullopt;
    }

    return std::make_shared<QJsonObject>(mCustoms.at(pCircuitId));
}

void CustomsLibrary::AddCustomJson(const QJsonObject& pJson)
{
#warning At some point, there should be a warning if a newer version of a circuit is detected
    qDebug() << "Inserting custom logic" << pJson["file"].toString();
    mCustoms.insert(std::make_pair(CircuitId(pJson["uuid"].toString(), pJson["timestamp"].toInt()), pJson));
}

void CustomsLibrary::Clear()
{
    mCustoms.clear();
}

QJsonArray CustomsLibrary::ExportJson() const
{
    QJsonArray customs;

    for (const auto& obj : mCustoms)
    {
        customs.append(obj.second);
    }

    return customs;
}
