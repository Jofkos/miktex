/**
 * @file internal.cpp
 * @author Christian Schenk
 * @brief Internal definitions
 *
 * @copyright Copyright © 1996-2023 Christian Schenk
 *
 * This file is part of the MiKTeX Core Library.
 *
 * The MiKTeX Core Library is licensed under GNU General Public License version
 * 2 or any later version.
 */

#pragma once

#if defined(ENABLE_OPENSSL)
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3)
#include <openssl/provider.h>
#endif
#endif

#include <miktex/Trace/TraceStream>

#include <miktex/Core/Quoter>
#include <miktex/Core/Session>

#if MIKTEX_UNIX
#  define USE_WINDOWS_REGISTRY 0
#else
#  define USE_WINDOWS_REGISTRY 1
#endif

#define FIND_FILE_PREFER_RELATIVE_PATH_NAMES 1
#define FIND_FILE_DONT_TRIGGER_INSTALLER_IF_ALL 1

 // experimental
#define USE_CODECVT_UTF8 1

#define CORE_INTERNAL_BEGIN_NAMESPACE           \
namespace MiKTeX {                              \
  namespace ABF3880A6239B84E87DC7E727A8BBFD4 {

#define CORE_INTERNAL_END_NAMESPACE             \
  }                                             \
}

#define BEGIN_ANONYMOUS_NAMESPACE namespace {
#define END_ANONYMOUS_NAMESPACE }

CORE_INTERNAL_BEGIN_NAMESPACE;

#define REPORT_EVENTS 0

#if defined(MIKTEX_WINDOWS)
#  define DEBUG_BREAK() DebugBreak()
#else
#  define DEBUG_BREAK()
#endif

#if !defined(UNUSED)
#  if !defined(NDEBUG)
#    define UNUSED(x)
#  else
#    define UNUSED(x) static_cast<void>(x)
#  endif
#endif

#if !defined(UNUSED_ALWAYS)
#  define UNUSED_ALWAYS(x) static_cast<void>(x)
#endif

#define BUF_TOO_SMALL() MIKTEX_INTERNAL_ERROR()

#define INVALID_ARGUMENT(argumentName, argumentValue) MIKTEX_FATAL_ERROR_2(T_("MiKTeX encountered an internal error."), argumentName, argumentValue)

#define OUT_OF_MEMORY(function) MIKTEX_INTERNAL_ERROR()

#define UNIMPLEMENTED() MIKTEX_INTERNAL_ERROR()

#if defined(_MSC_VER)
#define UNSUPPORTED_PLATFORM()                                          \
  __assume(false)
#else
#define UNSUPPORTED_PLATFORM()                                          \
  MIKTEX_ASSERT(false)
#endif

#define T_(x) MIKTEXTEXT(x)

#define Q_(x) MiKTeX::Core::Quoter<char>(x).GetData()

#if defined(MIKTEX_WINDOWS)
#  define WU_(x) MiKTeX::Util::CharBuffer<char>(x).GetData()
#  define UW_(x) MiKTeX::Util::CharBuffer<wchar_t>(x).GetData()
#endif

#define MIKTEXINTERNALFUNC(type) type
#define MIKTEXINTERNALVAR(type) type

#define ARRAY_SIZE(buf) (sizeof(buf) / sizeof(buf[0]))

#define MAKE_SEARCH_PATH(dir)                   \
  TEXMF_PLACEHOLDER                             \
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING        \
  dir

#define CFG_MACRO_NAME_BINDIR "bindir"
#define CFG_MACRO_NAME_ENGINE "engine"
#define CFG_MACRO_NAME_LOCALFONTDIRS "localfontdirs"
#define CFG_MACRO_NAME_PSFONTDIRS "psfontdirs"
#define CFG_MACRO_NAME_OTFDIRS "otfdirs"
#define CFG_MACRO_NAME_PROGNAME "progname"
#define CFG_MACRO_NAME_TTFDIRS "ttfdirs"
#define CFG_MACRO_NAME_WINDIR "windir"

const unsigned FNDB_PAGESIZE = 0x1000;

#define TEXMF_PLACEHOLDER "%R"

#define CURRENT_DIRECTORY "."

#define PARENT_DIRECTORY ".."

#define PARENT_PARENT_DIRECTORY                 \
  PARENT_DIRECTORY                              \
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING        \
  PARENT_DIRECTORY

#define PARENT_PARENT_PARENT_DIRECTORY          \
  PARENT_DIRECTORY                              \
  MIKTEX_PATH_DIRECTORY_DELIMITER_STRING        \
  PARENT_PARENT_DIRECTORY

#if defined(MIKTEX_WINDOWS)
const char PATH_DELIMITER = ';';
#define PATH_DELIMITER_STRING ";"
#else
const char PATH_DELIMITER = ':';
#define PATH_DELIMITER_STRING ":"
#endif

const char* const RECURSION_INDICATOR = "//";
const size_t RECURSION_INDICATOR_LENGTH = 2;
const char* const SESSIONSVC = "sessionsvc";

// The virtual TEXMF root MPM_ROOT_PATH is assigned to the MiKTeX
// package manager.  We make sure that MPM_ROOT_PATH is a valid path
// name.  On the other hand, it must not interfere with an existing
// file system.
#if defined(MIKTEX_WINDOWS)
// An UNC path with an impossible share name suits our needs: `['
// isn't a valid character in a share name (KB236388)
const char* const COMMON_MPM_ROOT_PATH = "\\\\MiKTeX\\[MPM]";
const char* const USER_MPM_ROOT_PATH = "\\\\MiKTeX\\]MPM[";
const size_t MPM_ROOT_PATH_LEN_ = 14;
#else
const char* const COMMON_MPM_ROOT_PATH = "//MiKTeX/[MPM]";
const char* const USER_MPM_ROOT_PATH = "//MiKTeX/]MPM[";
const size_t MPM_ROOT_PATH_LEN_ = 14;
#endif

#define MPM_ROOT_PATH                           \
  IsAdminMode()                                 \
  ? COMMON_MPM_ROOT_PATH                        \
  : USER_MPM_ROOT_PATH

#if defined(MIKTEX_DEBUG)
#define MPM_ROOT_PATH_LEN                                               \
  static_cast<size_t>(MIKTEX_ASSERT(MiKTeX::Util::StrLen(MPM_ROOT_PATH) \
                             == MPM_ROOT_PATH_LEN_),                    \
                      MPM_ROOT_PATH_LEN_)
#else
const size_t MPM_ROOT_PATH_LEN = MPM_ROOT_PATH_LEN_;
#endif

#if defined(ENABLE_OPENSSL)
using BIO_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
#if defined(LIBRESSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER < 0x10100000L
using EVP_MD_CTX_ptr = std::unique_ptr<EVP_MD_CTX, decltype(&::EVP_MD_CTX_destroy)>;
#else
using EVP_MD_CTX_ptr = std::unique_ptr<EVP_MD_CTX, decltype(&::EVP_MD_CTX_free)>;
#endif
using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
void FatalOpenSSLError();
RSA_ptr LoadPublicKey_OpenSSL(const MiKTeX::Util::PathName& publicKeyFile);
#endif

void CreateDirectoryPath(const MiKTeX::Util::PathName& path);

bool FileIsOnROMedia(const char* path);

bool GetCrtErrorMessage(int functionResult, std::string& errorMessage);

MiKTeX::Util::PathName GetHomeDirectory();

bool FixProgramSearchPath(const std::string& oldPath, const MiKTeX::Util::PathName& binDir_, bool checkCompetition, std::string& newPath, bool& competition);

#if defined(MIKTEX_WINDOWS)
bool GetUserProfileDirectory(MiKTeX::Util::PathName& path);
#endif

#if defined(MIKTEX_WINDOWS)
void SetTimesInternal(HANDLE handle, time_t creationTime, time_t lastAccessTime, time_t lastWriteTime);
#endif

#if defined(MIKTEX_WINDOWS)
bool GetSystemFontDirectory(MiKTeX::Util::PathName& path);
#endif

#if defined(MIKTEX_WINDOWS)
bool GetUserFontDirectory(MiKTeX::Util::PathName& path);
#endif

#if defined(MIKTEX_WINDOWS)
bool GetWindowsErrorMessage(unsigned long functionResult, std::string& errorMessage);
#endif

const char* GetFileNameExtension(const char* path);

enum class CryptoLib
{
    None,
    OpenSSL
};

CryptoLib GetCryptoLib();

bool HaveEnvironmentString(const char* name);

bool GetEnvironmentString(const std::string& name, std::string& value);

bool IsExplicitlyRelativePath(const char* path);

std::string MakeSearchPath(const std::vector<MiKTeX::Util::PathName>& vec);

void RemoveDirectoryDelimiter(char* path);

#if defined(MIKTEX_WINDOWS) && REPORT_EVENTS
bool ReportMiKTeXEvent(unsigned short eventType, unsigned long eventId, ...);
#endif

#if defined(MIKTEX_WINDOWS)
void TraceWindowsError(const char* windowsFunction, unsigned long functionResult, const char* info, const char* sourceFileName, int sourceLine);
#endif

const char* GetShortSourceFile(const char* sourceFileName);

inline bool IsValidTimeT(std::time_t time)
{
    return time != static_cast<std::time_t>(0) && time != static_cast<std::time_t>(-1);
}

inline void DbgView(const std::string& s)
{
#if defined(_WIN32)
    OutputDebugStringW(UW_("MiKTeX Core: " + s));
#endif
}

class memstreambuf:
    public std::streambuf
{
public:
    memstreambuf(const unsigned char* mem, size_t len)
    {
        char* s = reinterpret_cast<char*>(const_cast<unsigned char*>(mem));
        setg(s, s, s + len);
    }
};

template<class VALTYPE> class AutoRestore
{

public:

    AutoRestore(VALTYPE& val):
        oldVal(val),
        pVal(&val)
    {
    }

    ~AutoRestore()
    {
        *pVal = oldVal;
    }

private:

    VALTYPE oldVal;
    VALTYPE* pVal;
};

template<class VALTYPE> class Optional
{

public:

    Optional():
        hasValue(false)
    {
    }

    Optional(const VALTYPE& value):
        value(value),
        hasValue(true)
    {
    }

    bool HasValue() const
    {
        return hasValue;
    }

    operator bool() const
    {
        return HasValue();
    }

    const VALTYPE& GetValue() const
    {
        if (!hasValue)
        {
            MIKTEX_UNEXPECTED();
        }
        return value;
    }

    const VALTYPE& operator*() const
    {
        return GetValue();
    }

private:

    bool hasValue;
    VALTYPE value;
};

struct StringComparerIgnoringCase
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return MiKTeX::Util::StringCompare(lhs.c_str(), rhs.c_str(), true) < 0;
    }
};

inline FILE* FdOpen(const MiKTeX::Util::PathName& path, int fd, const char* mode)
{
#if defined(_MSC_VER)
    FILE* stream = _fdopen(fd, mode);
#else
    FILE* stream = fdopen(fd, mode);
#endif
    if (stream == nullptr)
    {
        MIKTEX_FATAL_CRT_ERROR_2("fdopen", "path", path.ToString());
    }
    return stream;
}

inline FILE* FdOpen(int fd, const char* mode)
{
#if defined(_MSC_VER)
    FILE* stream = _fdopen(fd, mode);
#else
    FILE* stream = fdopen(fd, mode);
#endif
    if (stream == nullptr)
    {
        MIKTEX_FATAL_CRT_ERROR("fdopen");
    }
    return stream;
}

inline bool EndsWith(const std::string& s, const std::string& suffix)
{
    return s.length() >= suffix.length() &&
        s.compare(s.length() - suffix.length(), suffix.length(), suffix) == 0;
}

/* _________________________________________________________________________ */

CORE_INTERNAL_END_NAMESPACE;

using namespace MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4;

#undef MIKTEXINTERNALFUNC
#define MIKTEXINTERNALFUNC(type) \
  type MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::
#define MIKTEXINTERNALFUNC2(type, callconv) \
  type callconv MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::

#undef MIKTEXINTERNALVAR
#define MIKTEXINTERNALVAR(type) \
  type MiKTeX::ABF3880A6239B84E87DC7E727A8BBFD4::

#if 0
#  define MIKTEXSTATICFUNC(type) static type
#  define MIKTEXSTATICFUNC2(type, callconv) static type callconv
#else
#  define MIKTEXSTATICFUNC(type) type
#  define MIKTEXSTATICFUNC2(type, callconv) type callconv
#endif

#define MIKTEXPERMANENTVAR(type) static type
