// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/ListThreshold.hpp
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
 * \file ListThreshold.hpp
 * \brief The ListThreshold class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef IMS_DATA_LISTTHRESHOLD_HPP
#define IMS_DATA_LISTTHRESHOLD_HPP

#include <IMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListThreshold_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace IMS_Data
{

    /**
     * \class ListThreshold
     * \brief Implementation of the ListThreshold class
     */
    class ListThreshold: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for ListThreshold
         */
        ListThreshold();
        /**
         * \brief The destructor for ListThreshold
         */
        virtual ~ListThreshold();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes

        // References
        /**
         * \brief To get the list of Thresholds
         * \return A reference to a list of Thresholds
         **/
        ::ecorecpp::mapping::EList< ::IMS_Data::Threshold >& getThreshold();

        /*PROTECTED REGION ID(ListThreshold) START*/
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

        /*PROTECTED REGION ID(ListThresholdImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes


        // References

        ::ecorecpp::mapping::out_ptr< ::ecorecpp::mapping::EList<
                ::IMS_Data::Threshold > > m_Threshold;

    };

} // IMS_Data

#endif // IMS_DATA_LISTTHRESHOLD_HPP
