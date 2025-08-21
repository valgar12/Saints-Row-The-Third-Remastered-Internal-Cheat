#pragma once
#include "includes.h"

class Mem {
    struct Entry {
        void* addr;
        std::vector<uint8_t> original;
        std::vector<uint8_t> patched;
        size_t len;
        int ID;
        bool isPatched = false;
    };

    std::vector<Entry> entries;

public:
    bool Nop(void* addr, size_t len, int ID) {
        if (!addr) return false;
        bool exists = false;
        int index = 0;

        for (int i = 0; i < entries.size(); i++)
        {
            if (entries[i].ID == ID)
            {
                exists = true;
                index = i;
            }
        }

        DWORD oldProtect;
        if (!VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

        if (!exists)
        {
            Entry e;
            e.addr = addr;
            e.len = len;
            e.original.reserve(len);
            e.patched.reserve(len);
            e.ID = ID;
            e.isPatched = true;

            auto p = reinterpret_cast<uint8_t*>(addr);
            for (size_t i = 0; i < len; ++i) {
                e.original.push_back(p[i]);
                e.patched.push_back(0x90);
                p[i] = 0x90;
            }

            DWORD tmp;
            if (!VirtualProtect(addr, len, oldProtect, &tmp)) {
                for (size_t i = 0; i < len; ++i) p[i] = e.original[i];
                return false;
            }

            entries.push_back(std::move(e));
        }
        else
        {
            entries[index].isPatched = true;

            auto p = reinterpret_cast<uint8_t*>(addr);
            for (size_t i = 0; i < len; ++i) {
                p[i] = 0x90;
            }

            DWORD tmp;
            if (!VirtualProtect(addr, len, oldProtect, &tmp)) {
                for (size_t i = 0; i < len; ++i) p[i] = entries[index].original[i];
                return false;
            }
        }
        
        return true;
    }

    bool Patch(void* addr, const std::vector<uint8_t>& bytes, int ID, size_t len = 0) {
        if (!addr) return false;
        if (len == 0) len = bytes.size();
        if (len < 1) return false;

        size_t bytesSize = bytes.size();

        bool exists = false;
        int index = 0;

        for (int i = 0; i < entries.size(); i++)
        {
            if (entries[i].ID == ID)
            {
                exists = true;
                index = i;
            }
        }

        DWORD oldProtect;
        if (!VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

        if (!exists)
        {
            Entry e;
            e.addr = addr;
            e.len = len;
            e.original.reserve(len);
            e.patched.reserve(len);
            e.ID = ID;
            e.isPatched = true;

            auto p = reinterpret_cast<uint8_t*>(addr);
            for (size_t i = 0; i < len; ++i) {
                e.original.push_back(p[i]);

                if (i > bytesSize - 1)
                {
                    e.patched.push_back(0x90);
                    p[i] = 0x90;
                }
                else
                {
                    e.patched.push_back(bytes[i]);
                    p[i] = bytes[i];
                }

            }

            DWORD tmp;
            if (!VirtualProtect(addr, len, oldProtect, &tmp)) {
                for (size_t i = 0; i < len; ++i) p[i] = e.original[i];
                return false;
            }

            entries.push_back(std::move(e));
        }
        else
        {
            entries[index].isPatched = true;

            auto p = reinterpret_cast<uint8_t*>(addr);
            for (size_t i = 0; i < len; ++i) {

                if (i > bytesSize - 1)
                {
                    p[i] = 0x90;
                }
                else
                {
                    p[i] = bytes[i];
                }

            }

            DWORD tmp;
            if (!VirtualProtect(addr, len, oldProtect, &tmp)) {
                for (size_t i = 0; i < len; ++i) p[i] = entries[index].original[i];
                return false;
            }
        }
        
        return true;
    }

    bool RestoreByID(int ID) {
        if (entries.empty()) return true;

        for (int i = 0; i < entries.size(); i++) {

            auto e = entries[i];
            if (e.ID != ID) continue;
            if (!e.isPatched) return true;

            DWORD oldProtect;
            if (!VirtualProtect(e.addr, e.len, PAGE_EXECUTE_READWRITE, &oldProtect))
                return false;

            auto p = reinterpret_cast<uint8_t*>(e.addr);
            for (size_t i = 0; i < e.len; ++i) p[i] = e.original[i];

            DWORD tmp;
            if (!VirtualProtect(e.addr, e.len, oldProtect, &tmp))
                return false;
            entries[i].isPatched = false;
            break;
        }

        

        return true;
    }

    bool isEntryPatched(int ID)
    {
        for (auto& e : entries)
        {
            if (e.ID == ID && e.isPatched == true) return true;
        }

        return false;
    }
};