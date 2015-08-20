/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <float.h>
#include "gazebo/common/Console.hh"
#include "gazebo/physics/plugin/PluginSurfaceParams.hh"

using namespace gazebo;
using namespace physics;

//////////////////////////////////////////////////
PluginSurfaceParams::PluginSurfaceParams()
  : SurfaceParams(),
    bounce(0), bounceThreshold(100000),
    kp(1000000000000), kd(1), cfm(0), erp(0.2),
    maxVel(0.01), minDepth(0),
    slip1(0), slip2(0)
{
}

//////////////////////////////////////////////////
PluginSurfaceParams::~PluginSurfaceParams()
{
}

//////////////////////////////////////////////////
void PluginSurfaceParams::Load(sdf::ElementPtr _sdf)
{
  // Load parent class
  SurfaceParams::Load(_sdf);

  if (!_sdf)
    gzerr << "Surface _sdf is NULL" << std::endl;
  else
  {
    {
      sdf::ElementPtr bounceElem = _sdf->GetElement("bounce");
      if (!bounceElem)
        gzerr << "Surface bounce sdf member is NULL" << std::endl;
      else
      {
        this->bounce = bounceElem->Get<double>("restitution_coefficient");
        if (this->bounce < 0)
        {
          gzwarn << "bounce restitution_coefficient ["
                 << this->bounce
                 << "] < 0, so it will not be applied by Plugin."
                 << std::endl;
        }
        else if (this->bounce > 1)
        {
          gzwarn << "bounce restitution_coefficient ["
                 << this->bounce
                 << "] > 1, which is outside the recommended range."
                 << std::endl;
        }
        this->bounceThreshold = bounceElem->Get<double>("threshold");
      }
    }

    {
      sdf::ElementPtr frictionElem = _sdf->GetElement("friction");
      if (!frictionElem)
        gzerr << "Surface friction sdf member is NULL" << std::endl;
      else
      {
        gzerr << "Surface friction plugin physics sdf member is"
              << " not defined." << std::endl;
        this->frictionPyramid.SetMuPrimary(1);
        this->frictionPyramid.SetMuSecondary(1);
        this->frictionPyramid.direction1 = math::Vector3(1, 0, 0);
        this->slip1 = 0;
        this->slip2 = 0;
      }
    }

    {
      sdf::ElementPtr contactElem = _sdf->GetElement("contact");
      if (!contactElem)
        gzerr << "Surface contact sdf member is NULL" << std::endl;
      else
      {
        gzerr << "Surface contact plugin physics sdf member is "
              << "not defined." << std::endl;
        this->kp = 1e6;
        this->kd = 1.0;
        this->cfm = 0.0;
        this->erp = 0.2;
        this->maxVel = 100.0;
        this->minDepth = 0.0;
      }
    }
  }
}

/////////////////////////////////////////////////
void PluginSurfaceParams::FillMsg(msgs::Surface &_msg)
{
  SurfaceParams::FillMsg(_msg);

  _msg.mutable_friction()->set_mu(this->frictionPyramid.GetMuPrimary());
  _msg.mutable_friction()->set_mu2(this->frictionPyramid.GetMuSecondary());
  _msg.mutable_friction()->set_slip1(this->slip1);
  _msg.mutable_friction()->set_slip2(this->slip2);
  msgs::Set(_msg.mutable_friction()->mutable_fdir1(),
            this->frictionPyramid.direction1);

  _msg.set_restitution_coefficient(this->bounce);
  _msg.set_bounce_threshold(this->bounceThreshold);

  _msg.set_soft_cfm(this->cfm);
  _msg.set_soft_erp(this->erp);
  _msg.set_kp(this->kp);
  _msg.set_kd(this->kd);
  _msg.set_max_vel(this->maxVel);
  _msg.set_min_depth(this->minDepth);
}

/////////////////////////////////////////////////
void PluginSurfaceParams::ProcessMsg(const msgs::Surface &_msg)
{
  SurfaceParams::ProcessMsg(_msg);

  if (_msg.has_friction())
  {
    if (_msg.friction().has_mu())
      this->frictionPyramid.SetMuPrimary(_msg.friction().mu());
    if (_msg.friction().has_mu2())
      this->frictionPyramid.SetMuSecondary(_msg.friction().mu2());
    if (_msg.friction().has_slip1())
      this->slip1 = _msg.friction().slip1();
    if (_msg.friction().has_slip2())
      this->slip2 = _msg.friction().slip2();
    if (_msg.friction().has_fdir1())
      this->frictionPyramid.direction1 =
        msgs::Convert(_msg.friction().fdir1());
  }

  if (_msg.has_restitution_coefficient())
    this->bounce = _msg.restitution_coefficient();
  if (_msg.has_bounce_threshold())
    this->bounceThreshold = _msg.bounce_threshold();
  if (_msg.has_soft_cfm())
    this->cfm = _msg.soft_cfm();
  if (_msg.has_soft_erp())
    this->erp = _msg.soft_erp();
  if (_msg.has_kp())
    this->kp = _msg.kp();
  if (_msg.has_kd())
    this->kd = _msg.kd();
  if (_msg.has_max_vel())
    this->maxVel = _msg.max_vel();
  if (_msg.has_min_depth())
    this->minDepth = _msg.min_depth();
}
