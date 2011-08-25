// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/FMS_DataPackage.cpp
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

#include <FMS_Data/FMS_DataPackage.hpp>

using namespace ::FMS_Data;

std::auto_ptr< ::FMS_Data::FMS_DataPackage > FMS_DataPackage::s_instance;

::FMS_Data::FMS_DataPackage_ptr FMS_DataPackage::_instance()
{
    if (!s_instance.get())
    {
        new FMS_DataPackage();
    }
    return s_instance.get();
}
