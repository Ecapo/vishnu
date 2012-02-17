// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ProgressOptions.hpp
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
 * \file ProgressOptions.hpp
 * \brief The ProgressOptions class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef TMS_DATA_PROGRESSOPTIONS_HPP
#define TMS_DATA_PROGRESSOPTIONS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
/*PROTECTED REGION ID(ProgressOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    /**
     * \class ProgressOptions
     * \brief Implementation of the ProgressOptions class
     */
    class VISHNU_API_LIB ProgressOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ProgressOptions
         */
        ProgressOptions();
        /**
         * \brief The destructor for ProgressOptions
         */
        virtual ~ProgressOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the jobId
         * \return The jobId attribute value
         **/
        ::ecore::EString const& getJobId() const;
        /**
         * \brief To set the jobId
         * \param _jobId The jobId value
         **/
        void setJobId(::ecore::EString const& _jobId);

        /**
         * \brief To get the jobOwner
         * \return The jobOwner attribute value
         **/
        ::ecore::EString const& getJobOwner() const;
        /**
         * \brief To set the jobOwner
         * \param _jobOwner The jobOwner value
         **/
        void setJobOwner(::ecore::EString const& _jobOwner);

        // References


        /*PROTECTED REGION ID(ProgressOptions) START*/
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

        /*PROTECTED REGION ID(ProgressOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_jobId;

        ::ecore::EString m_jobOwner;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_PROGRESSOPTIONS_HPP
