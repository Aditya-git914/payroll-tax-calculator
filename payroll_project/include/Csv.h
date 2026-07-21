#pragma once
#include <string>
#include <vector>
#include <sstream>

// Small header-only CSV helper: proper quoting so names/departments
// containing commas or quotes still round-trip correctly.
namespace csv {

inline std::string escape(const std::string& field) {
    bool needsQuotes = field.find(',') != std::string::npos ||
                        field.find('"') != std::string::npos ||
                        field.find('\n') != std::string::npos;
    if (!needsQuotes) return field;

    std::string out = "\"";
    for (char c : field) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += "\"";
    return out;
}

inline std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    cur += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                cur += c;
            }
        } else {
            if (c == '"') {
                inQuotes = true;
            } else if (c == ',') {
                fields.push_back(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }
    }
    fields.push_back(cur);
    return fields;
}

template <typename... Args>
inline std::string joinRow(Args&&... args) {
    std::ostringstream oss;
    bool first = true;
    auto append = [&](const std::string& s) {
        if (!first) oss << ",";
        oss << escape(s);
        first = false;
    };
    (append(args), ...);
    return oss.str();
}

} // namespace csv
