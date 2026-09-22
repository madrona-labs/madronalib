/*
Copyright (C) 2021 by ODDSound Ltd. info@oddsound.com

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
*/

#include "libMTSClient.h"
#include <math.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__TOS_WIN__) || defined(_MSC_VER)
#define MTS_ESP_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef HRESULT (WINAPI* SHGetKnownFolderPathFunc) (const GUID*, DWORD, HANDLE, PWSTR*);
typedef void (WINAPI* CoTaskMemFreeFunc) (LPVOID);
#else
#include <dlfcn.h>
#endif

const static int libMTSVersion = 0x00010003;

const static double ln2 = 0.693147180559945309417;
const static double ratioToSemitones = 17.31234049066756088832; // 12.0 / log(2.0)

typedef void (*mts_void__void)(void);
typedef bool (*mts_bool__void)(void);
typedef int (*mts_int__void)(void);
typedef bool (*mts_bool__char_schar)(char, signed char);
typedef const double *(*mts_pConstDouble__void)(void);
typedef const double *(*mts_pConstDouble__schar)(signed char);
typedef bool (*mts_bool__schar)(signed char);
typedef const char *(*mts_pConstChar__void)(void);
typedef double (*mts_double__void)(void);
typedef signed char (*mts_schar__void)(void);

struct mtsclientglobal
{
    mtsclientglobal() 
    : RegisterClient(0)
    , DeregisterClient(0)
    , HasMaster(0)
    , GetVersionNumber(0)
    , ShouldFilterNote(0)
    , ShouldFilterNoteMultiChannel(0)
    , GetTuning(0)
    , GetMultiChannelTuning(0)
    , UseMultiChannelTuning(0)
    , GetScaleName(0)
    , GetPeriodRatio(0)
    , GetMapSize(0)
    , GetMapStartKey(0)
    , GetRefKey(0)
    , esp_retuning(0)
    , handle(0)
    {
        for (int i = 0; i < 128; i++)
            iet[i] = 1. / (440.0 * pow(2.0, (i - 69.0) / 12.0));
        
        load_lib();
        
        if (GetTuning)
            esp_retuning = GetTuning();
        
        for (int i = 0; i < 16; i++)
            multi_channel_esp_retuning[i] = GetMultiChannelTuning ? GetMultiChannelTuning(static_cast<signed char>(i)) : 0;
    }
    
    inline bool isOnline() const {return esp_retuning && HasMaster && HasMaster();}
    
    // interface to lib
    mts_void__void RegisterClient;
    mts_void__void DeregisterClient;
    mts_bool__void HasMaster;
    mts_int__void GetVersionNumber;
    mts_bool__char_schar ShouldFilterNote;
    mts_bool__char_schar ShouldFilterNoteMultiChannel;
    mts_pConstDouble__void GetTuning;
    mts_pConstDouble__schar GetMultiChannelTuning;
    mts_bool__schar UseMultiChannelTuning;
    mts_pConstChar__void GetScaleName;
    mts_double__void GetPeriodRatio;
    mts_schar__void GetMapSize;
    mts_schar__void GetMapStartKey;
    mts_schar__void GetRefKey;
    
    // tuning tables
    double iet[128];
    const double *esp_retuning;
    const double *multi_channel_esp_retuning[16];
    
#ifdef MTS_ESP_WIN
    void load_lib()
    {
        SHGetKnownFolderPathFunc SHGetKnownFolderPath = 0;
        CoTaskMemFreeFunc CoTaskMemFree = 0;
        
        HMODULE shell32Module = GetModuleHandleW(L"Shell32.dll");
        HMODULE ole32Module = GetModuleHandleW(L"Ole32.dll");
        
        if (shell32Module) 
            SHGetKnownFolderPath = (SHGetKnownFolderPathFunc)GetProcAddress(shell32Module, "SHGetKnownFolderPath");
        
        if (ole32Module) 
            CoTaskMemFree = (CoTaskMemFreeFunc)GetProcAddress(ole32Module, "CoTaskMemFree");
        
        if (SHGetKnownFolderPath && CoTaskMemFree)
        {
            const GUID FOLDERID_ProgramFilesCommonGUID = {0xF7F1ED05, 0x9F6D, 0x47A2, 0xAA, 0xAE, 0x29, 0xD3, 0x17, 0xC6, 0xF0, 0x66};
            PWSTR cf = NULL;
            if (SHGetKnownFolderPath(&FOLDERID_ProgramFilesCommonGUID, 0, 0, &cf) >= 0)
            {
                WCHAR buffer[MAX_PATH];
                buffer[0] = L'\0';
                if (cf)
                    wcsncpy(buffer, cf, MAX_PATH);
                CoTaskMemFree(cf);
                buffer[MAX_PATH - 1] = L'\0';
                const WCHAR *libpath = L"\\MTS-ESP\\LIBMTS.dll";
                DWORD cfLen = wcslen(buffer);
                wcsncat(buffer, libpath, MAX_PATH - cfLen - 1);
                handle = LoadLibraryW(buffer);
                if (!handle)
                    return;
            }
            else 
            {
                CoTaskMemFree(cf);
                return;
            }
        }
        else
        {
            return;
        }
        
        RegisterClient                  = (mts_void__void)          GetProcAddress(handle, "MTS_RegisterClient");
        DeregisterClient                = (mts_void__void)          GetProcAddress(handle, "MTS_DeregisterClient");
        HasMaster                       = (mts_bool__void)          GetProcAddress(handle, "MTS_HasMaster");
        GetVersionNumber                = (mts_int__void)           GetProcAddress(handle, "MTS_GetVersionNumber");
        ShouldFilterNote                = (mts_bool__char_schar)    GetProcAddress(handle, "MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    = (mts_bool__char_schar)    GetProcAddress(handle, "MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       = (mts_pConstDouble__void)  GetProcAddress(handle, "MTS_GetTuningTable");
        GetMultiChannelTuning           = (mts_pConstDouble__schar) GetProcAddress(handle, "MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           = (mts_bool__schar)         GetProcAddress(handle, "MTS_UseMultiChannelTuning");
        GetScaleName                    = (mts_pConstChar__void)    GetProcAddress(handle, "MTS_GetScaleName");
        GetPeriodRatio                  = (mts_double__void)        GetProcAddress(handle, "MTS_GetPeriodRatio");
        GetMapSize                      = (mts_schar__void)         GetProcAddress(handle, "MTS_GetMapSize");
        GetMapStartKey                  = (mts_schar__void)         GetProcAddress(handle, "MTS_GetMapStartKey");
        GetRefKey                       = (mts_schar__void)         GetProcAddress(handle, "MTS_GetRefKey");
    }
    
    ~mtsclientglobal() 
    {
        if (handle)
            FreeLibrary(handle);
    }
    
    HINSTANCE handle;
#else
    void load_lib()
    {
        if (!(handle = dlopen("/Library/Application Support/MTS-ESP/libMTS.dylib", RTLD_NOW)) &&
            !(handle = dlopen("/usr/local/lib/libMTS.so", RTLD_NOW)))
        {
            return;
        }
        
        RegisterClient                  = (mts_void__void)          dlsym(handle, "MTS_RegisterClient");
        DeregisterClient                = (mts_void__void)          dlsym(handle, "MTS_DeregisterClient");
        HasMaster                       = (mts_bool__void)          dlsym(handle, "MTS_HasMaster");
        GetVersionNumber                = (mts_int__void)           dlsym(handle, "MTS_GetVersionNumber");
        ShouldFilterNote                = (mts_bool__char_schar)    dlsym(handle, "MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    = (mts_bool__char_schar)    dlsym(handle, "MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       = (mts_pConstDouble__void)  dlsym(handle, "MTS_GetTuningTable");
        GetMultiChannelTuning           = (mts_pConstDouble__schar) dlsym(handle, "MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           = (mts_bool__schar)         dlsym(handle, "MTS_UseMultiChannelTuning");
        GetScaleName                    = (mts_pConstChar__void)    dlsym(handle, "MTS_GetScaleName");
        GetPeriodRatio                  = (mts_double__void)        dlsym(handle, "MTS_GetPeriodRatio");
        GetMapSize                      = (mts_schar__void)         dlsym(handle, "MTS_GetMapSize");
        GetMapStartKey                  = (mts_schar__void)         dlsym(handle, "MTS_GetMapStartKey");
        GetRefKey                       = (mts_schar__void)         dlsym(handle, "MTS_GetRefKey");
    }
    
    ~mtsclientglobal()
    {
        if (handle)
            dlclose(handle);
    }
    
    void *handle;
#endif
};

static mtsclientglobal global;

struct MTSClient
{
    struct Tuning
    {
        enum {eRatioValid = 1, eSemitonesValid = 1 << 1};
        int flags;
        double freq; // always valid
        double ratio;
        double semitones;
    };
    
    MTSClient()
    : tuningName("12-TET")
    , periodRatioLocal(2.0)
    , periodSemitones(12.0)
    , mapSizeLocal(static_cast<signed char>(-1))
    , mapStartKeyLocal(static_cast<signed char>(-1))
    , supportsNoteFiltering(false)
    , supportsMultiChannelNoteFiltering(false)
    , supportsMultiChannelTuning(false)
    , freqRequestReceived(false)
    , receivedMTSSysEx(false)
    {
        for (int i = 0; i < 128; i++)
        {
            localFreqs[i] = 440.0 * pow(2.0, (i - 69.0) / 12.0);
            localTunings[i].flags = 0;
            localTunings[i].freq = localFreqs[i];
            globalTunings[i].flags = 0;
            globalTunings[i].freq = localFreqs[i];
        }
        
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 128; j++)
            {
                globalMultichannelTunings[i][j].flags = 0;
                globalMultichannelTunings[i][j].freq = localFreqs[i];
            }
        }
                
        if (global.RegisterClient)
            global.RegisterClient();
    }
    
    ~MTSClient()
    {
        if (global.DeregisterClient)
            global.DeregisterClient();
    }
    
    inline bool hasMaster() {return global.isOnline();}
    inline bool shouldUpdateLibrary() {return global.GetVersionNumber ? (global.GetVersionNumber() < libMTSVersion) : false;}
    
    inline double freq(char midinote, signed char midichannel)
    {
        int note = midinote & 127;
        int channel = midichannel & 15;
        
        freqRequestReceived = true;
        supportsMultiChannelTuning = !(midichannel & ~15);
        
        if (!global.isOnline())
            return localTunings[note].freq;
        
        if ((!supportsNoteFiltering || supportsMultiChannelNoteFiltering) &&
            supportsMultiChannelTuning &&
            global.UseMultiChannelTuning &&
            global.UseMultiChannelTuning(midichannel) &&
            global.multi_channel_esp_retuning[channel])
        {
            globalMultichannelTunings[channel][note].freq = global.multi_channel_esp_retuning[channel][note];
            globalMultichannelTunings[channel][note].flags = 0;
            return globalMultichannelTunings[channel][note].freq;
        }
        
        globalTunings[note].freq = global.esp_retuning[note];
        globalTunings[note].flags = 0;
        return globalTunings[note].freq;
    }
    
    inline double ratio(char midinote, signed char midichannel)
    {
        int note = midinote & 127;
        int channel = midichannel & 15;
        
        freqRequestReceived = true;
        supportsMultiChannelTuning = !(midichannel & ~15);
        
        if (!global.isOnline())
        {
            if (!receivedMTSSysEx)
                return 1.0;
            
            if (localTunings[note].flags & Tuning::eRatioValid)
                return localTunings[note].ratio;
            
            localTunings[note].ratio = localTunings[note].freq * global.iet[note];
            localTunings[note].flags |= Tuning::eRatioValid;
            return localTunings[note].ratio;
        }
        
        if ((!supportsNoteFiltering || supportsMultiChannelNoteFiltering) &&
            supportsMultiChannelTuning &&
            global.UseMultiChannelTuning &&
            global.UseMultiChannelTuning(midichannel) &&
            global.multi_channel_esp_retuning[channel])
        {
            double freq = global.multi_channel_esp_retuning[channel][note];
            
            if (globalMultichannelTunings[channel][note].freq == freq &&
                (globalMultichannelTunings[channel][note].flags & Tuning::eRatioValid))
            {
                return globalMultichannelTunings[channel][note].ratio;
            }
            
            globalMultichannelTunings[channel][note].freq = global.multi_channel_esp_retuning[channel][note];
            globalMultichannelTunings[channel][note].ratio = globalMultichannelTunings[channel][note].freq * global.iet[note];
            globalMultichannelTunings[channel][note].flags = Tuning::eRatioValid;
            return globalMultichannelTunings[channel][note].ratio;
        }
        
        double freq = global.esp_retuning[note];
        
        if (globalTunings[note].freq == freq &&
            (globalTunings[note].flags & Tuning::eRatioValid))
        {
            return globalTunings[note].ratio;
        }
        
        globalTunings[note].freq = global.esp_retuning[note];
        globalTunings[note].ratio = globalTunings[note].freq * global.iet[note];
        globalTunings[note].flags = Tuning::eRatioValid;
        return globalTunings[note].ratio;
    }
    
    inline double semitones(char midinote, signed char midichannel)
    {
        int note = midinote & 127;
        int channel = midichannel & 15;
        
        freqRequestReceived = true;
        supportsMultiChannelTuning = !(midichannel & ~15);
        
        if (!global.isOnline())
        {
            if (!receivedMTSSysEx)
                return 0.0;
            
            if (localTunings[note].flags & Tuning::eSemitonesValid)
                return localTunings[note].semitones;
            
            if (localTunings[note].flags & Tuning::eRatioValid)
            {
                localTunings[note].semitones = ratioToSemitones * log(localTunings[note].ratio);
                localTunings[note].flags |= Tuning::eSemitonesValid;
                return localTunings[note].semitones;
            }
            
            localTunings[note].ratio = localTunings[note].freq * global.iet[note];
            localTunings[note].semitones = ratioToSemitones * log(localTunings[note].ratio);
            localTunings[note].flags |= Tuning::eRatioValid | Tuning::eSemitonesValid;
            return localTunings[note].semitones;
        }
        
        if ((!supportsNoteFiltering || supportsMultiChannelNoteFiltering) &&
            supportsMultiChannelTuning &&
            global.UseMultiChannelTuning &&
            global.UseMultiChannelTuning(midichannel) &&
            global.multi_channel_esp_retuning[channel])
        {
            double freq = global.multi_channel_esp_retuning[channel][note];
            
            if (globalMultichannelTunings[channel][note].freq == freq)
            {
                if (globalMultichannelTunings[channel][note].flags & Tuning::eSemitonesValid)
                    return globalMultichannelTunings[channel][note].semitones;
                
                if (globalMultichannelTunings[channel][note].flags & Tuning::eRatioValid)
                {
                    globalMultichannelTunings[channel][note].semitones = ratioToSemitones * log(globalMultichannelTunings[channel][note].ratio);
                    globalMultichannelTunings[channel][note].flags |= Tuning::eSemitonesValid;
                    return globalMultichannelTunings[channel][note].semitones;
                }
            }
            
            globalMultichannelTunings[channel][note].freq = freq;
            globalMultichannelTunings[channel][note].ratio = freq * global.iet[note];
            globalMultichannelTunings[channel][note].semitones = ratioToSemitones * log(globalMultichannelTunings[channel][note].ratio);
            globalMultichannelTunings[channel][note].flags = Tuning::eRatioValid | Tuning::eSemitonesValid;
            return globalMultichannelTunings[channel][note].semitones;
        }
        
        double freq = global.esp_retuning[note];
        
        if (globalTunings[note].freq == freq)
        {
            if (globalTunings[note].flags & Tuning::eSemitonesValid)
                return globalTunings[note].semitones;
            
            if (globalTunings[note].flags & Tuning::eRatioValid)
            {
                globalTunings[note].semitones = ratioToSemitones * log(globalTunings[note].ratio);
                globalTunings[note].flags |= Tuning::eSemitonesValid;
                return globalTunings[note].semitones;
            }
        }
        
        globalTunings[note].freq = freq;
        globalTunings[note].ratio = freq * global.iet[note];
        globalTunings[note].semitones = ratioToSemitones * log(globalTunings[note].ratio);
        globalTunings[note].flags = Tuning::eRatioValid | Tuning::eSemitonesValid;
        return globalTunings[note].semitones;
    }
    
    inline bool shouldFilterNote(char midinote, signed char midichannel)
    {
        supportsNoteFiltering = true;
        supportsMultiChannelNoteFiltering = !(midichannel & ~15);
        
        if (!freqRequestReceived)
            supportsMultiChannelTuning = supportsMultiChannelNoteFiltering; // assume it supports multi channel tuning until a request is received for a frequency and can verify
        
        if (!global.isOnline())
            return false;
        
        if (supportsMultiChannelNoteFiltering &&
            supportsMultiChannelTuning &&
            global.UseMultiChannelTuning &&
            global.UseMultiChannelTuning(midichannel))
        {
            return global.ShouldFilterNoteMultiChannel ? global.ShouldFilterNoteMultiChannel(midinote & 127, midichannel) : false;
        }
        
        return global.ShouldFilterNote ? global.ShouldFilterNote(midinote & 127, midichannel) : false;
    }
    
    inline char freqToNote(double freq, signed char midichannel)
    {
        bool online = global.isOnline();
        bool multiChannel = false;
        const double *freqs = online ? global.esp_retuning : localFreqs;
        
        if (online &&
            !(midichannel & ~15) &&
            global.UseMultiChannelTuning &&
            global.UseMultiChannelTuning(midichannel) &&
            global.multi_channel_esp_retuning[midichannel & 15])
        {
            freqs = global.multi_channel_esp_retuning[midichannel & 15];
            multiChannel = true;
        }
        
        int iLower = 0;
        int iUpper = 0;
        double dLower = 0.0;
        double dUpper = 0.0;
        
        for (int i = 0; i < 128; i++)
        {
            if (online)
            {
                if (multiChannel && 
                    global.ShouldFilterNoteMultiChannel &&
                    global.ShouldFilterNoteMultiChannel(static_cast<char>(i), midichannel))
                {
                    continue;
                }
                
                if (!multiChannel &&
                    global.ShouldFilterNote &&
                    global.ShouldFilterNote(static_cast<char>(i), midichannel))
                {
                    continue;
                }
            }
            
            double d = freqs[i] - freq;
            
            if (d == 0.0)
                return static_cast<char>(i);
            
            if (d < 0.0)
            {
                if (dLower == 0.0 || d > dLower)
                {
                    dLower=d;
                    iLower=i;
                }
            }
            else if (dUpper == 0.0 || d < dUpper)
            {
                dUpper = d;
                iUpper = i;
            }
        }
        
        if (dLower == 0.0)
            return static_cast<char>(iUpper);
        
        if (dUpper == 0.0 || iLower == iUpper)
            return static_cast<char>(iLower);
        
        double fmid = freqs[iLower] * pow(2.0, 0.5 * (log(freqs[iUpper] / freqs[iLower]) / ln2));
        return freq < fmid ? static_cast<char>(iLower) : static_cast<char>(iUpper);
    }
    
    inline char freqToNote(double freq, signed char *midichannel)
    {
        if (!midichannel) 
            return freqToNote(freq, static_cast<signed char>(-1));
        
        if (global.isOnline() && global.UseMultiChannelTuning)
        {
            int channelsInUse[16];
            int nMultiChannels = 0;
            for (int i = 0; i < 16; i++)
                if (global.UseMultiChannelTuning(i) && global.multi_channel_esp_retuning[i])
                    channelsInUse[nMultiChannels++] = i;
            
            if (nMultiChannels > 0)
            {
                const int nFreqs = 128 * nMultiChannels;
                int iLower = 0;
                int iUpper = 0;
                int channel = 0;
                int note = 0;
                double dLower = 0.0;
                double dUpper = 0.0;
                
                for (int i = 0; i < nFreqs; i++)
                {
                    channel = channelsInUse[i >> 7];
                    note = i & 127;
                    
                    if (global.ShouldFilterNoteMultiChannel &&
                        global.ShouldFilterNoteMultiChannel(static_cast<char>(note), static_cast<signed char>(channel)))
                    {
                        continue;
                    }
                    
                    double d = global.multi_channel_esp_retuning[channel][note] - freq;
                    
                    if (d == 0.0)
                    {
                        *midichannel = static_cast<signed char>(channel);
                        return static_cast<char>(note);
                    }
                    
                    if (d < 0.0)
                    {
                        if (dLower == 0.0 || d > dLower)
                        {
                            dLower = d;
                            iLower = i;
                        }
                    }
                    else if (dUpper == 0.0 || d < dUpper)
                    {
                        dUpper = d;
                        iUpper = i;
                    }
                }
                
                if (dLower == 0.0)
                {
                    *midichannel = static_cast<signed char>(channelsInUse[iUpper >> 7]);
                    return static_cast<char>(iUpper & 127);
                }
                
                if (dUpper == 0.0 || iLower == iUpper)
                {
                    *midichannel = static_cast<signed char>(channelsInUse[iLower >> 7]);
                    return static_cast<char>(iLower & 127);
                }
                
                double fLower = global.multi_channel_esp_retuning[channelsInUse[iLower >> 7]][iLower & 127];
                double fUpper = global.multi_channel_esp_retuning[channelsInUse[iUpper >> 7]][iUpper & 127];
                double fmid = fLower * pow(2.0, 0.5 * (log(fUpper / fLower) / ln2));
                
                if (freq < fmid)
                {
                    *midichannel = static_cast<signed char>(channelsInUse[iLower >> 7]);
                    return static_cast<char>(iLower & 127);
                }
                
                *midichannel = static_cast<signed char>(channelsInUse[iUpper >> 7]);
                return static_cast<char>(iUpper & 127);
            }
        }
        
        *midichannel = static_cast<signed char>(0);
        return freqToNote(freq, static_cast<signed char>(0));
    }
    
    inline void parseMIDIData(const unsigned char *buffer, int len)
    {
        int sysex_ctr = 0;
        int sysex_value = 0;
        int note = 0;
        int numTunings = 0;
        /*int bank = -1, prog = 0, checksum = 0, deviceID = 0; short int channelBitmap = 0; bool realtime = false;*/ // unused for now
        
        eSysexState state = eIgnoring;
        eMTSFormat format = eBulk;
        for (int i = 0; i < len; i++)
        {
            unsigned char b = buffer[i];
            if (b == 0xF7)
            {
                state = eIgnoring;
                continue;
            }
            
            if (b > 0x7F && b != 0xF0)
                continue;
            
            switch (state)
            {
                case eIgnoring:
                    if (b == 0xF0)
                        state = eMatchingSysex;
                    break;
                case eMatchingSysex:
                    sysex_ctr = 0;
                    if (b == 0x7E)
                        state = eSysexValid;
                    else if (b == 0x7F)
                    {
                        /*realtime = true;*/
                        state = eSysexValid;
                    }
                    else 
                    {
                        state = eIgnoring;
                    }
                    break;
                case eSysexValid:
                    switch (sysex_ctr++) // handle device ID
                    {
                        case 0:
                            /*deviceID = b;*/
                            break;
                        case 1: 
                            if (b == 0x08)
                                state = eMatchingMTS;
                            break;
                        default: // it's not an MTS message
                            state = eIgnoring;
                            break;
                    }
                    break;
                case eMatchingMTS:
                    sysex_ctr = 0;
                    switch (b)
                    {
                        case 0: 
                            format = eRequest;
                            state = eMatchingProg;
                            break;
                        case 1: 
                            format = eBulk;
                            state = eMatchingProg;
                            break;
                        case 2: 
                            format = eSingle;
                            state = eMatchingProg;
                            break;
                        case 3: 
                            format = eRequest; 
                            state = eMatchingBank; 
                            break;
                        case 4:
                            format = eBulk; 
                            state = eMatchingBank; 
                            break;
                        case 5:
                            format = eScaleOctOneByte; 
                            state = eMatchingBank; 
                            break;
                        case 6:
                            format = eScaleOctTwoByte; 
                            state = eMatchingBank; 
                            break;
                        case 7:
                            format = eSingle; 
                            state = eMatchingBank; 
                            break;
                        case 8:
                            format = eScaleOctOneByteExt; 
                            state = eMatchingChannel; 
                            break;
                        case 9:
                            format = eScaleOctTwoByteExt; 
                            state = eMatchingChannel; 
                            break;
                        default: // it's not a valid MTS format
                            state = eIgnoring;
                            break;
                    }
                    break;
                case eMatchingBank:
                    /*bank = b;*/
                    state = eMatchingProg;
                    break;
                case eMatchingProg:
                    /*prog = b;*/
                    if (format == eSingle)
                    {
                        state = eNumTunings;
                    }
                    else
                    {
                        state = eTuningName;
                        tuningName[0] = '\0';
                    }
                    break;
                case eTuningName:
                    tuningName[sysex_ctr] = static_cast<char>(b);
                    if (++sysex_ctr >= 16)
                    {
                        tuningName[16] = '\0';
                        sysex_ctr = 0;
                        state = eTuningData;
                    }
                    break;
                case eNumTunings:
                    numTunings = b;
                    sysex_ctr = 0;
                    state = eTuningData;
                    break;
                case eMatchingChannel:
                    switch (sysex_ctr++)
                    {
                        case 0: 
                            /*for (int j = 14; j < 16; j++) channelBitmap |= (1 << j);*/
                            break;
                        case 1: 
                            /*for (int j = 7; j < 14; j++) channelBitmap |= (1 << j);*/
                            break;
                        case 2: 
                            /*for (int j = 0; j < 7; j++) channelBitmap |= (1 << j);*/
                            sysex_ctr = 0;
                            state = eTuningData;
                            break;
                    }
                    break;
                case eTuningData:
                    switch (format)
                    {
                        case eBulk:
                            sysex_value = (sysex_value << 7) | b;
                            sysex_ctr++;
                            if ((sysex_ctr & 3) == 3)
                            {
                                if (!(note == 0x7F && sysex_value == 16383))
                                    updateTuning(note, (sysex_value >> 14) & 127, (sysex_value & 16383) / 16383.0);
                                sysex_value = 0;
                                sysex_ctr++;
                                if (++note >= 128)
                                    state = eCheckSum;
                            }
                            break;
                        case eSingle:
                            sysex_value = (sysex_value << 7) | b;
                            sysex_ctr++;
                            if (!(sysex_ctr & 3))
                            {
                                if (!(note == 0x7F && sysex_value == 16383))
                                    updateTuning((sysex_value >> 21) & 127, (sysex_value >> 14) & 127, (sysex_value & 16383) / 16383.0);
                                sysex_value = 0;
                                if (++note >= numTunings)
                                    state = eIgnoring;
                            }
                            break;
                        case eScaleOctOneByte: 
                        case eScaleOctOneByteExt:
                            for (int j = sysex_ctr; j < 128; j += 12)
                                updateTuning(j, j, (static_cast<double>(b) - 64.0) * 0.01);
                            if (++sysex_ctr >= 12)
                                state = format == eScaleOctOneByte ? eCheckSum : eIgnoring;
                            break;
                        case eScaleOctTwoByte: 
                        case eScaleOctTwoByteExt:
                            sysex_value = (sysex_value << 7) | b;
                            sysex_ctr++;
                            if (!(sysex_ctr & 1))
                            {
                                double detune = (static_cast<double>(sysex_value & 16383) - 8192.0) / (sysex_value > 8192 ? 8191.0 : 8192.0);
                                for (int j = note; j < 128; j += 12)
                                    updateTuning(j, j, detune);
                                if (++note >= 12)
                                    state = format == eScaleOctTwoByte ? eCheckSum : eIgnoring;
                            }
                            break;
                        default: 
                            state = eIgnoring;
                            break;
                    }
                    break;
                case eCheckSum:
                    /*checksum = b;*/
                    state = eIgnoring;
                    break;
            }
        }
        
        if (format == eScaleOctOneByte || format == eScaleOctTwoByte || format == eScaleOctOneByteExt || format == eScaleOctTwoByteExt)
        {
            mapSizeLocal = static_cast<signed char>(12);
            mapStartKeyLocal = static_cast<signed char>(60);
        }
        else
        {
            mapSizeLocal = static_cast<signed char>(-1);
            mapStartKeyLocal = static_cast<signed char>(-1);
        }
    }
    
    inline void updateTuning(int note, int retuneNote, double detune)
    {
        if (note < 0 || note > 127 || retuneNote < 0 || retuneNote > 127)
            return;
        receivedMTSSysEx = true;
        localFreqs[note] = 440.0 * pow(2.0, ((retuneNote + detune) - 69.0) / 12.0);
        if (localFreqs[note] != localTunings[note].freq)
        {
            localTunings[note].freq = localFreqs[note];
            localTunings[note].flags = 0;
        }
    }
    
    inline bool hasReceivedMTSSysEx() {return receivedMTSSysEx;}
    
    const char *getScaleName() {return (global.isOnline() && global.GetScaleName) ? global.GetScaleName() : tuningName;}
    
    double getPeriodRatio() {return (global.isOnline() && global.GetPeriodRatio) ? global.GetPeriodRatio() : 2.0;}
    double getPeriodSemitones()
    {
        double periodRatio = getPeriodRatio();
        if (periodRatio != periodRatioLocal)
        {
            periodSemitones = ratioToSemitones * log(periodRatio);
            periodRatioLocal = periodRatio;
        }
        return periodSemitones;
    }
    
    signed char getMapSize() {return (global.isOnline() && global.GetMapSize) ? global.GetMapSize() : mapSizeLocal;}
    signed char getMapStartKey() {return (global.isOnline() && global.GetMapStartKey) ? global.GetMapStartKey() : mapStartKeyLocal;}
    signed char getRefKey() {return (global.isOnline() && global.GetRefKey) ? global.GetRefKey() : static_cast<signed char>(-1);}
    
    enum eSysexState {eIgnoring = 0, eMatchingSysex, eSysexValid, eMatchingMTS, eMatchingBank, eMatchingProg, eMatchingChannel, eTuningName, eNumTunings, eTuningData, eCheckSum};
    enum eMTSFormat {eRequest = 0, eBulk, eSingle, eScaleOctOneByte, eScaleOctTwoByte, eScaleOctOneByteExt, eScaleOctTwoByteExt};

    double localFreqs[128];
    Tuning localTunings[128];
    Tuning globalTunings[128];
    Tuning globalMultichannelTunings[16][128];
    
    char tuningName[17];
    
    double periodRatioLocal;
    double periodSemitones;
    
    signed char mapSizeLocal;
    signed char mapStartKeyLocal;
    
    bool supportsNoteFiltering;
    bool supportsMultiChannelNoteFiltering;
    bool supportsMultiChannelTuning;
    bool freqRequestReceived;
    bool receivedMTSSysEx;
};

static char freqToNoteET(double freq)
{
    if (isnan(freq))
        return static_cast<char>(60);
    
    // filter out frequencies that are almost exactly at MIDI boundaries
    if (freq < 8.3) // corresponds to 0 < n < 0.5
        return static_cast<char>(0);
    if (freq > 12400.0) // corresponds to 126.5 < n < 127
        return static_cast<char>(127);

    static const double INV_440 = 1.0 / 440.0;
    long n = 69 + lround(12.0 * log2(freq * INV_440));
    return static_cast<char>(n);
}

// exported functions:
MTSClient* MTS_RegisterClient()                                                         {return new MTSClient;}
void MTS_DeregisterClient(MTSClient *c)                                                 {delete c;}
bool MTS_HasMaster(MTSClient *c)                                                        {return c ? c->hasMaster() : false;}
bool MTS_Client_ShouldUpdateLibrary(MTSClient *c)                                       {return c ? c->shouldUpdateLibrary() : false;}
bool MTS_ShouldFilterNote(MTSClient *c, char midinote, signed char midichannel)         {return c ? c->shouldFilterNote(midinote & 127, midichannel) : false;}
double MTS_NoteToFrequency(MTSClient *c, char midinote, signed char midichannel)        {return c ? c->freq(midinote, midichannel) : (1.0 / global.iet[midinote & 127]);}
double MTS_RetuningAsRatio(MTSClient *c, char midinote, signed char midichannel)        {return c ? c->ratio(midinote, midichannel) : 1.0;}
double MTS_RetuningInSemitones(MTSClient *c, char midinote, signed char midichannel)    {return c ? c->semitones(midinote, midichannel) : 0.0;}
char MTS_FrequencyToNote(MTSClient *c, double freq, signed char midichannel)            {return c ? c->freqToNote(freq, midichannel) : freqToNoteET(freq);}
char MTS_FrequencyToNoteAndChannel(MTSClient *c, double freq, signed char *midichannel) {if (c) return c->freqToNote(freq, midichannel); if (midichannel) *midichannel = 0; return freqToNoteET(freq);}
const char *MTS_GetScaleName(MTSClient *c)                                              {return c ? c->getScaleName() : "";}
double MTS_GetPeriodRatio(MTSClient *c)                                                 {return c ? c->getPeriodRatio() : 2.0;}
double MTS_GetPeriodSemitones(MTSClient *c)                                             {return c ? c->getPeriodSemitones() : 12.0;}
signed char MTS_GetMapSize(MTSClient *c)                                                {return c ? c->getMapSize() : static_cast<signed char>(-1);}
signed char MTS_GetMapStartKey(MTSClient *c)                                            {return c ? c->getMapStartKey() : static_cast<signed char>(-1);}
signed char MTS_GetRefKey(MTSClient *c)                                                 {return c ? c->getRefKey() : static_cast<signed char>(-1);}
void MTS_ParseMIDIDataU(MTSClient *c, const unsigned char *buffer, int len)             {if (c) c->parseMIDIData(buffer, len);}
void MTS_ParseMIDIData(MTSClient *c, const signed char *buffer, int len)                {if (c) c->parseMIDIData(reinterpret_cast<const unsigned char*>(buffer), len);}
bool MTS_HasReceivedMTSSysEx(MTSClient *c)                                              {return c ? c->hasReceivedMTSSysEx() : false;}
