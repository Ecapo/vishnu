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
 * FMS_Data/FileStat.hpp
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
 * \file FileStat.hpp
 * \brief The FileStat class
 * \author Generated file
 * \date 31/03/2011
 */

#ifndef FMS_DATA_FILESTAT_HPP
#define FMS_DATA_FILESTAT_HPP

#include <FMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(FileStat_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace FMS_Data
{

    /**
     * \class FileStat
     * \brief Implementation of the FileStat class
     */
    class FileStat: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for FileStat
         */
        FileStat();
        /**
         * \brief The destructor for FileStat
         */
        virtual ~FileStat();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the path
         * \return The path attribute value
         **/
        ::ecore::EString const& getPath() const;
        /**
         * \brief To set the path
         * \param _path The path value
         **/
        void setPath(::ecore::EString const& _path);

        /**
         * \brief To get the owner
         * \return The owner attribute value
         **/
        ::ecore::EString const& getOwner() const;
        /**
         * \brief To set the owner
         * \param _owner The owner value
         **/
        void setOwner(::ecore::EString const& _owner);

        /**
         * \brief To get the group
         * \return The group attribute value
         **/
        ::ecore::EString const& getGroup() const;
        /**
         * \brief To set the group
         * \param _group The group value
         **/
        void setGroup(::ecore::EString const& _group);

        /**
         * \brief To get the perms
         * \return The perms attribute value
         **/
        ::ecore::EInt getPerms() const;
        /**
         * \brief To set the perms
         * \param _perms The perms value
         **/
        void setPerms(::ecore::EInt _perms);

        /**
         * \brief To get the uid
         * \return The uid attribute value
         **/
        ::ecore::ELong getUid() const;
        /**
         * \brief To set the uid
         * \param _uid The uid value
         **/
        void setUid(::ecore::ELong _uid);

        /**
         * \brief To get the gid
         * \return The gid attribute value
         **/
        ::ecore::ELong getGid() const;
        /**
         * \brief To set the gid
         * \param _gid The gid value
         **/
        void setGid(::ecore::ELong _gid);

        /**
         * \brief To get the size
         * \return The size attribute value
         **/
        ::ecore::ELong getSize() const;
        /**
         * \brief To set the size
         * \param _size The size value
         **/
        void setSize(::ecore::ELong _size);

        /**
         * \brief To get the atime
         * \return The atime attribute value
         **/
        ::ecore::ELong getAtime() const;
        /**
         * \brief To set the atime
         * \param _atime The atime value
         **/
        void setAtime(::ecore::ELong _atime);

        /**
         * \brief To get the mtime
         * \return The mtime attribute value
         **/
        ::ecore::ELong getMtime() const;
        /**
         * \brief To set the mtime
         * \param _mtime The mtime value
         **/
        void setMtime(::ecore::ELong _mtime);

        /**
         * \brief To get the ctime
         * \return The ctime attribute value
         **/
        ::ecore::ELong getCtime() const;
        /**
         * \brief To set the ctime
         * \param _ctime The ctime value
         **/
        void setCtime(::ecore::ELong _ctime);

        /**
         * \brief To get the type
         * \return The type attribute value
         **/
        ::FMS_Data::FileType getType() const;
        /**
         * \brief To set the type
         * \param _type The type value
         **/
        void setType(::FMS_Data::FileType _type);

        // References


        /*PROTECTED REGION ID(FileStat) START*/
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

        /*PROTECTED REGION ID(FileStatImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_path;

        ::ecore::EString m_owner;

        ::ecore::EString m_group;

        ::ecore::EInt m_perms;

        ::ecore::ELong m_uid;

        ::ecore::ELong m_gid;

        ::ecore::ELong m_size;

        ::ecore::ELong m_atime;

        ::ecore::ELong m_mtime;

        ::ecore::ELong m_ctime;

        ::FMS_Data::FileType m_type;

        // References

    };

} // FMS_Data

#endif // FMS_DATA_FILESTAT_HPP
