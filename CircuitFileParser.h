#ifndef CIRCUITFILEPARSER_H
#define CIRCUITFILEPARSER_H

#include "RuntimeConfigParser.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCborMap>

class CircuitFileParser : public QObject
{
    Q_OBJECT
public:
    CircuitFileParser(RuntimeConfigParser& pRuntimeConfigParser);

    /// \brief Load the circuit in the given file
    /// \param pFileInfo: The file to load
    void LoadJson(const QFileInfo& pFileInfo);

    /// \brief Saves the circuit to current file
    /// \param pJson: The JSON to save into the file
    void SaveJson(const QJsonObject& pJson);

    /// \brief Saves the circuit to the given file
    /// \param pFileInfo: The file to save into
    /// \param pJson: The JSON to save into the file
    void SaveJsonAs(const QFileInfo& pFileInfo, const QJsonObject& pJson);

    void ResetCurrentFileInfo(void);

    /// \brief Returns true, if there is a known file path to save the current circuit into
    /// \return True, if there is a known file path to save the current circuit into
    bool IsFileOpen(void) const;

    /// \brief Getter for the current file info, if it exists
    /// \return The current file info, if it exists
    std::optional<QFileInfo> GetFileInfo(void) const;

    /// \brief Returns true, if the circuit has been modified after the last save or load
    /// \return True, if the circuit has been modified after the last save or load
    bool IsCircuitModified(void) const;

    /// \brief Emits a signal if the circuit is newly modified
    void MarkAsModified(void);

signals:
    void LoadCircuitFileSuccessSignal(const QFileInfo& pFileInfo, const QJsonObject& pJson);
    void LoadCircuitFileFailedSignal(const QFileInfo& pFileInfo);

    void SaveCircuitFileSuccessSignal(const QFileInfo& pFileInfo);
    void SaveCircuitFileFailedSignal(const QFileInfo& pFileInfo);

    /// \brief Emitted when the circuit goes from an unmodified (saved or empty) to a modified (unsaved) state
    void CircuitModifiedSignal(void);

protected:
    std::optional<QFileInfo> mCurrentFile;

    bool mIsCircuitModified;

    RuntimeConfigParser& mRuntimeConfigParser;
};

#endif // CIRCUITFILEPARSER_H
