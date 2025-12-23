#include "JsonFileWatcher.hpp"
#include <thread>

JsonFileWatcher::JsonFileWatcher(const std::string &filePath) : m_FilePath(filePath), m_StopFlag(false) {
    if (fs::exists(m_FilePath)) {
        m_LastWriteTime = fs::last_write_time(m_FilePath);
    }
}

JsonFileWatcher::~JsonFileWatcher() {
    Stop();
}

void JsonFileWatcher::StartMonitoring(std::function<void()> on_change_cb, std::chrono::milliseconds interval) {
  m_MonitoringFuture = std::async(std::launch::async, [this, on_change_cb, interval]() {
        while (!m_StopFlag.load()) {
            CheckForUpdate(on_change_cb);
            std::this_thread::sleep_for(interval);
        }
    });
}

void JsonFileWatcher::Stop() {
    m_StopFlag.store(true);
    if (m_MonitoringFuture.valid()) {
        m_MonitoringFuture.get();
    }
}

std::string_view JsonFileWatcher::GetFilePath() const {
    return m_FilePath;
}

void JsonFileWatcher::CheckForUpdate(std::function<void()> on_change_cb) {
    try {
        auto currentWriteTime = fs::last_write_time(m_FilePath);
        if (currentWriteTime != m_LastWriteTime) {
            m_LastWriteTime = currentWriteTime;
            on_change_cb();
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}
