#pragma once

/*
	Casual Game Engine (dnyCasualGameEngine) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "shared.h"

/* Key/mouse binding management */
namespace Input {
	class CInputMgr {
	private:
		struct binding_s {
			std::wstring wszIdent;
			int vCode;
		};

		std::vector<binding_s> m_vBindings;
	public:
		CInputMgr() {}
		~CInputMgr() { this->m_vBindings.clear(); }

		void AddKeyBinding(const std::wstring& wszIdent, int vCode)
		{
			//Add key binding

			binding_s sBinding;
			sBinding.wszIdent = wszIdent;
			sBinding.vCode = vCode;

			this->m_vBindings.push_back(sBinding);
		}

		int GetKeyBindingCode(const std::wstring& wszIdent)
		{
			//Get key binding code

			for (size_t i = 0; i < this->m_vBindings.size(); i++) {
				if (this->m_vBindings[i].wszIdent == wszIdent) {
					return this->m_vBindings[i].vCode;
				}
			}

			return 0;
		}

		void SetKeyBindingCode(const std::wstring& wszIdent, int vCode)
		{
			//Set key binding code

			for (size_t i = 0; i < this->m_vBindings.size(); i++) {
				if (this->m_vBindings[i].wszIdent == wszIdent) {
					this->m_vBindings[i].vCode = vCode;
				}
			}
		}
	};
}