#pragma once
/*
 * Cross-platform DLL library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (C) 2017-2025 Michael Lee(李登淳)
 *
 * Email:   lee@covariant.cn, mikecovlee@163.com
 * Github:  https://github.com/mikecovlee
 */
#include <stdexcept>
#include <string>

#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

static std::wstring utf8_to_wide(const std::string &s)
{
	if (s.empty())
		return std::wstring();
	int len = ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	if (len <= 0)
		return std::wstring();
	std::wstring ws(static_cast<size_t>(len), L'\0');
	::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &ws[0], len);
	if (!ws.empty() && ws.back() == L'\0')
		ws.pop_back();
	return ws;
}

static std::string wide_to_utf8(const std::wstring &ws)
{
	if (ws.empty())
		return std::string();
	int len = ::MultiByteToWideChar(CP_UTF8, 0, nullptr, 0, nullptr, 0); // dummy to satisfy msvc warnings
	(void)len;
	int bytes = ::WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (bytes <= 0)
		return std::string();
	std::string s(static_cast<size_t>(bytes), '\0');
	::WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, &s[0], bytes, nullptr, nullptr);
	if (!s.empty() && s.back() == '\0')
		s.pop_back();
	return s;
}

namespace cov {
	class dll final {
		HMODULE m_handle = nullptr;

	public:
		dll() = default;

		dll(const dll &) = delete;

		explicit dll(const std::string &path)
		{
			open(path);
		}

		~dll()
		{
			if (m_handle != nullptr)
				::FreeLibrary(m_handle);
		}

		dll &operator=(const dll &) = delete;

		bool is_open() const noexcept
		{
			return m_handle != nullptr;
		}

		void open(const std::string &path)
		{
			if (m_handle != nullptr)
				::FreeLibrary(m_handle);
			std::wstring wpath = utf8_to_wide(path);
			m_handle = ::LoadLibraryW(wpath.c_str());
			if (m_handle == nullptr) {
				static WCHAR szBuf[256];
				::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, ::GetLastError(),
				                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuf, sizeof(szBuf) / sizeof(WCHAR), nullptr);
				throw std::logic_error(wide_to_utf8(szBuf));
			}
		}

		void close()
		{
			if (m_handle != nullptr)
				::FreeLibrary(m_handle);
			m_handle = nullptr;
		}

		void *get_address(const std::string &method)
		{
			if (m_handle == nullptr)
				throw std::logic_error("Used an unopened file.");
			return reinterpret_cast<void *>(::GetProcAddress(m_handle, method.c_str()));
		}
	};
}
#else

#include <dlfcn.h>

#ifdef RTLD_DEEPBIND
// GNU extension
#define COVLIBDLL_DLOPEN_ARGUMENTS (RTLD_LAZY | RTLD_DEEPBIND)
#else
#define COVLIBDLL_DLOPEN_ARGUMENTS (RTLD_LAZY)
#endif

namespace cov {
	class dll final {
		void *m_handle = nullptr;

	public:
		dll() = default;

		dll(const dll &) = delete;

		dll(const std::string &path)
		{
			open(path);
		}

		~dll()
		{
			if (m_handle != nullptr)
				::dlclose(m_handle);
		}

		dll &operator=(const dll &) = delete;

		bool is_open() const noexcept
		{
			return m_handle != nullptr;
		}

		void open(const std::string &path)
		{
			if (m_handle != nullptr)
				::dlclose(m_handle);
			::dlerror();
			void *sym = ::dlopen(path.c_str(), COVLIBDLL_DLOPEN_ARGUMENTS);
			const char *err = ::dlerror();
			if (err != nullptr)
				throw std::logic_error(err);
			m_handle = sym;
		}

		void close()
		{
			if (m_handle != nullptr)
				::dlclose(m_handle);
			m_handle = nullptr;
		}

		void *get_address(const std::string &method)
		{
			if (m_handle == nullptr)
				throw std::logic_error("Used an unopened file.");
			return ::dlsym(m_handle, method.c_str());
		}
	};
}
#endif
