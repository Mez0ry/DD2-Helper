#ifndef __JSON_FILE_WATCHER_HPP__
#define __JSON_FILE_WATCHER_HPP__

#include <iostream>
#include <filesystem>
#include <future>
#include <chrono>
#include <atomic>

namespace fs = std::filesystem;

class JsonFileWatcher {
private:
    std::string m_FilePath;
    fs::file_time_type m_LastWriteTime;
    std::future<void> m_MonitoringFuture;
    std::atomic<bool> m_StopFlag;

public:
    JsonFileWatcher(const std::string& filePath);
    ~JsonFileWatcher();

    void StartMonitoring(std::function<void()> on_change_cb, std::chrono::milliseconds interval = std::chrono::milliseconds(200));
    void Stop();

    std::string_view GetFilePath() const;

private:
    void CheckForUpdate(std::function<void()> on_change_cb);
};

#endif //!__JSON_FILE_WATCHER_HPP__