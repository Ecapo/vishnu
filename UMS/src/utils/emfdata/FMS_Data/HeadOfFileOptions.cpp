// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/HeadOfFileOptions.cpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "HeadOfFileOptions.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "FMS_Data/FMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::FMS_Data;

// Default constructor
HeadOfFileOptions::HeadOfFileOptions()
{

    /*PROTECTED REGION ID(HeadOfFileOptionsImpl__HeadOfFileOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

HeadOfFileOptions::~HeadOfFileOptions()
{
}

/*PROTECTED REGION ID(HeadOfFileOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EInt HeadOfFileOptions::getNline() const
{
    return m_nline;
}

void HeadOfFileOptions::setNline(::ecore::EInt _nline)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nline = m_nline;
#endif
    m_nline = _nline;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::FMS_Data::FMS_DataPackage::_instance()->getHeadOfFileOptions__nline(),
                _old_nline,
                m_nline
        );
        eNotify(&notification);
    }
#endif
}

// References

