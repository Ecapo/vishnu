// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListMachineOptions.hpp
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
 * \file ListMachineOptions.hpp
 * \brief The ListMachineOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef UMS_DATA_LISTMACHINEOPTIONS_HPP
#define UMS_DATA_LISTMACHINEOPTIONS_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef WIN32
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(ListMachineOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListMachineOptions
     * \brief Implementation of the ListMachineOptions class
     */
    class VISHNU_API_LIB ListMachineOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListMachineOptions
         */
        ListMachineOptions();
        /**
         * \brief The destructor for ListMachineOptions
         */
        virtual ~ListMachineOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
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
         * \brief To get the listAllMachine
         * \return The listAllMachine attribute value
         **/
        ::ecore::EBoolean isListAllMachine() const;
        /**
         * \brief To set the listAllMachine
         * \param _listAllMachine The listAllMachine value
         **/
        void setListAllMachine(::ecore::EBoolean _listAllMachine);

        /**
         * \brief To get the machineId
         * \return The machineId attribute value
         **/
        ::ecore::EString const& getMachineId() const;
        /**
         * \brief To set the machineId
         * \param _machineId The machineId value
         **/
        void setMachineId(::ecore::EString const& _machineId);

        // References


        /*PROTECTED REGION ID(ListMachineOptions) START*/
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

        /*PROTECTED REGION ID(ListMachineOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_userId;

        ::ecore::EBoolean m_listAllMachine;

        ::ecore::EString m_machineId;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LISTMACHINEOPTIONS_HPP
