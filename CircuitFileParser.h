#ifndef CIRCUITFILEPARSER_H
#define CIRCUITFILEPARSER_H

#include "RuntimeConfigParser.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCborMap>

///
/// \brief The CircuitFileParser handles all circuit file loading and saving operations
///
class CircuitFileParser : public QObject
{
    Q_OBJECT
public:
    /// \brief Constructor for the CircuitFileParser
    /// \param pRuntimeConfigParser: Reference to the runtime config parser
    CircuitFileParser(RuntimeConfigParser& pRuntimeConfigParser);

    /// \brief Load the circuit in the given file
    /// \param pFileInfo: The file to load
    /// \param pIsFromRecents: Whether the file to open is from the recent files list
    void LoadJson(const QFileInfo& pFileInfo, bool pIsFromRecents = false);

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

    const QString& GetUuid(void) const;

signals:
    /// \brief Emitted when a circuit file has been opened successfully
    /// \param pFileInfo: Info about the opened file
    /// \param pJson: The file content as a QJsonObject reference
    void LoadCircuitFileSuccessSignal(const QFileInfo& pFileInfo, const QJsonObject& pJson);

    /// \brief Emitted when a circuit file could not be opened
    /// \param pFileInfo: Info about the file
    /// \param pIsFromRecents: Whether the file is from the recent files list
    void LoadCircuitFileFailedSignal(const QFileInfo& pFileInfo, bool pIsFromRecents);

    /// \brief Emitted when a circuit file has been saved successfully
    /// \param pFileInfo: Info about the saved file
    void SaveCircuitFileSuccessSignal(const QFileInfo& pFileInfo);

    /// \brief Emitted when a circuit file could not be saved
    /// \param pFileInfo: Info about the file
    void SaveCircuitFileFailedSignal(const QFileInfo& pFileInfo);

    /// \brief Emitted when the circuit goes from an unmodified (saved or empty) to a modified (unsaved) state
    void CircuitModifiedSignal(void);

protected:
    std::optional<QFileInfo> mCurrentFile;
    bool mIsCircuitModified;

    QString mUuid;

    RuntimeConfigParser& mRuntimeConfigParser;
};

#endif // CIRCUITFILEPARSER_H
