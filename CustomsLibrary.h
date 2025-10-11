/*!
 * \file CustomsLibrary.h
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

#ifndef CUSTOMSLIBRARY_H
#define CUSTOMSLIBRARY_H

#include "HelperStructures.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

///
/// \brief The CustomsLibrary class implements a library for custom logic components
///
class CustomsLibrary : public QObject
{
    Q_OBJECT
public:
    /// \brief Constructor for the custom library
    /// \param pParent: Pointer to the parent object
    explicit CustomsLibrary(QObject *pParent = nullptr);

    /// \brief Returns an optional library entry with the specified key, if it exists
    /// \param pCircuitId: The key for the custom logic JSON
    /// \return A library entry in JSON format or std::nullopt
    std::optional<std::shared_ptr<QJsonObject>> GetCustomJson(const CircuitId& pCircuitId) const;

    /// \brief Adds an entry to the library
    /// \param pJson: The JSON of the custom logic to include
    void AddCustomJson(const QJsonObject& pJson);

    /// \brief Clears all library entries
    void Clear(void);

    /// \brief Returns a JSON array which includes all library entries
    /// \return A JSON array which includes all library entries
    QJsonArray ExportJson(void) const;

protected:
    std::map<CircuitId, QJsonObject> mCustoms;
};

#endif // CUSTOMSLIBRARY_H
