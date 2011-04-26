// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListCmdOptions.hpp
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

/**
 * \file ListCmdOptions.hpp
 * \brief The ListCmdOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_LISTCMDOPTIONS_HPP
#define UMS_DATA_LISTCMDOPTIONS_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListCmdOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListCmdOptions
     * \brief Implementation of the ListCmdOptions class
     */
    class ListCmdOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListCmdOptions
         */
        ListCmdOptions();
        /**
         * \brief The destructor for ListCmdOptions
         */
        virtual ~ListCmdOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the adminListOption
         * \return The adminListOption attribute value
         **/
        ::ecore::EBoolean isAdminListOption() const;
        /**
         * \brief To set the adminListOption
         * \param _adminListOption The adminListOption value
         **/
        void setAdminListOption(::ecore::EBoolean _adminListOption);

        /**
         * \brief To get the userId
         * \return The userId attribute value
         **/
        ::ecore::EString const& getUserId() const;
        /**
         * \brief To set the userId
         * \param _userId The userId value
         **/
        void setUserId(::ecore::EString const& _userId);

        /**
         * \brief To get the sessionId
         * \return The sessionId attribute value
         **/
        ::ecore::EString const& getSessionId() const;
        /**
         * \brief To set the sessionId
         * \param _sessionId The sessionId value
         **/
        void setSessionId(::ecore::EString const& _sessionId);

        /**
         * \brief To get the startDateOption
         * \return The startDateOption attribute value
         **/
        ::ecore::ELong getStartDateOption() const;
        /**
         * \brief To set the startDateOption
         * \param _startDateOption The startDateOption value
         **/
        void setStartDateOption(::ecore::ELong _startDateOption);

        /**
         * \brief To get the endDateOption
         * \return The endDateOption attribute value
         **/
        ::ecore::ELong getEndDateOption() const;
        /**
         * \brief To set the endDateOption
         * \param _endDateOption The endDateOption value
         **/
        void setEndDateOption(::ecore::ELong _endDateOption);

        // References


        /*PROTECTED REGION ID(ListCmdOptions) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
        virtual void eUnset(::ecore::EInt _featureID);
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(ListCmdOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EBoolean m_adminListOption;

        ::ecore::EString m_userId;

        ::ecore::EString m_sessionId;

        ::ecore::ELong m_startDateOption;

        ::ecore::ELong m_endDateOption;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LISTCMDOPTIONS_HPP
