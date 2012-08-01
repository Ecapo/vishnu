/* This file is a part of VISHNU.

* Copyright SysFera SAS (2011) 

* contact@sysfera.com

* This software is a computer program whose purpose is to provide 
* access to distributed computing resources.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use, 
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info". 

* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability. 
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or 
* data to be ensured and,  more generally, to use and operate it in the 
* same conditions as regards security. 
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/Process.hpp
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
 * \file Process.hpp
 * \brief The Process class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_PROCESS_HPP
#define IMS_DATA_PROCESS_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(Process_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class Process
     * \brief Implementation of the Process class
     */
    class Process: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for Process
         */
        Process();
        /**
         * \brief The destructor for Process
         */
        virtual ~Process();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the processName
         * \return The processName attribute value
         **/
        ::ecore::EString const& getProcessName() const;
        /**
         * \brief To set the processName
         * \param _processName The processName value
         **/
        void setProcessName(::ecore::EString const& _processName);

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

        /**
         * \brief To get the dietId
         * \return The dietId attribute value
         **/
        ::ecore::EString const& getDietId() const;
        /**
         * \brief To set the dietId
         * \param _dietId The dietId value
         **/
        void setDietId(::ecore::EString const& _dietId);

        /**
         * \brief To get the state
         * \return The state attribute value
         **/
        ::IMS_Data::ProcessState getState() const;
        /**
         * \brief To set the state
         * \param _state The state value
         **/
        void setState(::IMS_Data::ProcessState _state);

        /**
         * \brief To get the timestamp
         * \return The timestamp attribute value
         **/
        ::ecore::ELong getTimestamp() const;
        /**
         * \brief To set the timestamp
         * \param _timestamp The timestamp value
         **/
        void setTimestamp(::ecore::ELong _timestamp);

        /**
         * \brief To get the script
         * \return The script attribute value
         **/
        ::ecore::EString const& getScript() const;
        /**
         * \brief To set the script
         * \param _script The script value
         **/
        void setScript(::ecore::EString const& _script);

        // References


        /*PROTECTED REGION ID(Process) START*/
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

        /*PROTECTED REGION ID(ProcessImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_processName;

        ::ecore::EString m_machineId;

        ::ecore::EString m_dietId;

        ::IMS_Data::ProcessState m_state;

        ::ecore::ELong m_timestamp;

        ::ecore::EString m_script;

        // References

    };

} // IMS_Data

#endif // IMS_DATA_PROCESS_HPP
