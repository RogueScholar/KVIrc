//
//   File : class_buttongroup.cpp
//   Creation date : Fri Jan 28 14:21:48 CEST 2005
//   by Tonino Imbesi(Grifisx) and Alessandro Carbone(Noldor)
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 1999-2005 Szymon Stefanek (pragma at kvirc dot net)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//


#include "class_buttongroup.h"
#include "kvi_error.h"
#include "kvi_debug.h"
#include "kvi_locale.h"
#include "kvi_iconmanager.h"

#include <QButtonGroup>
#include <QRadioButton>

/*
	@doc:	buttongroup
	@keyterms:
		buttongroup object class,
	@title:
		buttongroup class
	@type:
		class
	@short:
		Provides a buttongroup control.
	@inherits:
		[class]object[/class]
	@description:
		This object organizes buttons in a group.
	@functions:
		!fn: $addButton(<checkbutton or radiobutton:object>)
		Adds the given button to the button group.
		!fn: <object> $checkedButton()
		Returns the button group's checked button, or 0 if no buttons are checked.
*/

KVSO_BEGIN_REGISTERCLASS(KviKvsObject_buttongroup,"buttongroup","object")
	KVSO_REGISTER_HANDLER(KviKvsObject_buttongroup,"addButton",functionAddButton)
	KVSO_REGISTER_HANDLER(KviKvsObject_buttongroup,"checkedButton",functionCheckedButton)

KVSO_END_REGISTERCLASS(KviKvsObject_buttongroup)

KVSO_BEGIN_CONSTRUCTOR(KviKvsObject_buttongroup,KviKvsObject)
	m_iId=0;
	btnDict.setAutoDelete(false);
	m_pButtonGroup=new QButtonGroup();
KVSO_END_CONSTRUCTOR(KviKvsObject_buttongroup)


KVSO_BEGIN_DESTRUCTOR(KviKvsObject_buttongroup)
btnDict.clear();
delete m_pButtonGroup;
KVSO_END_CONSTRUCTOR(KviKvsObject_buttongroup)


bool KviKvsObject_buttongroup::functionAddButton(KviKvsObjectFunctionCall *c)
{
	KviKvsObject * pObject;
	kvs_hobject_t hObject;
	KVSO_PARAMETERS_BEGIN(c)
		KVSO_PARAMETER("button",KVS_PT_HOBJECT,0,hObject)
	KVSO_PARAMETERS_END(c)
	pObject=KviKvsKernel::instance()->objectController()->lookupObject(hObject);
	if (!pObject)
	{
		c->warning(__tr2qs("Widget parameter is not an object"));
		return true;
	}
	if (!pObject->object())
	{
		c->warning(__tr2qs("Widget parameter is not a valid object"));
		return true;
	}
	if(!pObject->object()->isWidgetType())
	{
		c->warning(__tr2qs("Can't add a non-widget object"));
		return true;
	}
	if(pObject->inherits("KviKvsObject_radiobutton") || pObject->inherits("KviKvsObject_checkbox")){
		m_pButtonGroup->addButton(((QRadioButton *)(pObject->object())),m_iId);
		c->returnValue()->setInteger(m_iId);
		btnDict.insert(m_iId,pObject);
		m_iId++;
	}
	else{
		c->warning(__tr2qs("Buttongroup support only checkbox and radiobox object"));
		return true;
	}
	return true;
}
bool KviKvsObject_buttongroup::functionCheckedButton(KviKvsObjectFunctionCall *c)
{
	int id=m_pButtonGroup->checkedId();
	if (id!=-1) c->returnValue()->setHObject(btnDict.find(id)->handle());
	else c->returnValue()->setNothing();
	return true;
}
