/* 
 * Copyright (c) 2007, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "base_bridge.h"
#include "grt/grt_manager.h"

/**
 * @file  base_bridge.cpp
 * @brief 
 */


bool BridgeBase::is_main_thread()
{
  return bec::GRTManager::get_instance_for(get_object()->get_grt())->in_main_thread();
}


void BridgeBase::run_later(const boost::function<void ()> &slot)
{
    bec::GRTManager::get_instance_for(get_object()->get_grt())->run_once_when_idle(this, slot);
}

