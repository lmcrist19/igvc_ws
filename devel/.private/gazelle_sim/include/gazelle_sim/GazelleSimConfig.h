//#line 2 "/opt/ros/noetic/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"
// *********************************************************
//
// File autogenerated for the gazelle_sim package
// by the dynamic_reconfigure package.
// Please do not edit.
//
// ********************************************************/

#ifndef __gazelle_sim__GAZELLESIMCONFIG_H__
#define __gazelle_sim__GAZELLESIMCONFIG_H__

#if __cplusplus >= 201103L
#define DYNAMIC_RECONFIGURE_FINAL final
#else
#define DYNAMIC_RECONFIGURE_FINAL
#endif

#include <dynamic_reconfigure/config_tools.h>
#include <limits>
#include <ros/node_handle.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <dynamic_reconfigure/ParamDescription.h>
#include <dynamic_reconfigure/Group.h>
#include <dynamic_reconfigure/config_init_mutex.h>
#include <boost/any.hpp>

namespace gazelle_sim
{
  class GazelleSimConfigStatics;

  class GazelleSimConfig
  {
  public:
    class AbstractParamDescription : public dynamic_reconfigure::ParamDescription
    {
    public:
      AbstractParamDescription(std::string n, std::string t, uint32_t l,
          std::string d, std::string e)
      {
        name = n;
        type = t;
        level = l;
        description = d;
        edit_method = e;
      }
      virtual ~AbstractParamDescription() = default;

      virtual void clamp(GazelleSimConfig &config, const GazelleSimConfig &max, const GazelleSimConfig &min) const = 0;
      virtual void calcLevel(uint32_t &level, const GazelleSimConfig &config1, const GazelleSimConfig &config2) const = 0;
      virtual void fromServer(const ros::NodeHandle &nh, GazelleSimConfig &config) const = 0;
      virtual void toServer(const ros::NodeHandle &nh, const GazelleSimConfig &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, GazelleSimConfig &config) const = 0;
      virtual void toMessage(dynamic_reconfigure::Config &msg, const GazelleSimConfig &config) const = 0;
      virtual void getValue(const GazelleSimConfig &config, boost::any &val) const = 0;
    };

    typedef boost::shared_ptr<AbstractParamDescription> AbstractParamDescriptionPtr;
    typedef boost::shared_ptr<const AbstractParamDescription> AbstractParamDescriptionConstPtr;

    // Final keyword added to class because it has virtual methods and inherits
    // from a class with a non-virtual destructor.
    template <class T>
    class ParamDescription DYNAMIC_RECONFIGURE_FINAL : public AbstractParamDescription
    {
    public:
      ParamDescription(std::string a_name, std::string a_type, uint32_t a_level,
          std::string a_description, std::string a_edit_method, T GazelleSimConfig::* a_f) :
        AbstractParamDescription(a_name, a_type, a_level, a_description, a_edit_method),
        field(a_f)
      {}

      T GazelleSimConfig::* field;

      virtual void clamp(GazelleSimConfig &config, const GazelleSimConfig &max, const GazelleSimConfig &min) const override
      {
        if (config.*field > max.*field)
          config.*field = max.*field;

        if (config.*field < min.*field)
          config.*field = min.*field;
      }

      virtual void calcLevel(uint32_t &comb_level, const GazelleSimConfig &config1, const GazelleSimConfig &config2) const override
      {
        if (config1.*field != config2.*field)
          comb_level |= level;
      }

      virtual void fromServer(const ros::NodeHandle &nh, GazelleSimConfig &config) const override
      {
        nh.getParam(name, config.*field);
      }

      virtual void toServer(const ros::NodeHandle &nh, const GazelleSimConfig &config) const override
      {
        nh.setParam(name, config.*field);
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, GazelleSimConfig &config) const override
      {
        return dynamic_reconfigure::ConfigTools::getParameter(msg, name, config.*field);
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const GazelleSimConfig &config) const override
      {
        dynamic_reconfigure::ConfigTools::appendParameter(msg, name, config.*field);
      }

      virtual void getValue(const GazelleSimConfig &config, boost::any &val) const override
      {
        val = config.*field;
      }
    };

    class AbstractGroupDescription : public dynamic_reconfigure::Group
    {
      public:
      AbstractGroupDescription(std::string n, std::string t, int p, int i, bool s)
      {
        name = n;
        type = t;
        parent = p;
        state = s;
        id = i;
      }

      virtual ~AbstractGroupDescription() = default;

      std::vector<AbstractParamDescriptionConstPtr> abstract_parameters;
      bool state;

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &config) const =0;
      virtual void updateParams(boost::any &cfg, GazelleSimConfig &top) const= 0;
      virtual void setInitialState(boost::any &cfg) const = 0;


      void convertParams()
      {
        for(std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = abstract_parameters.begin(); i != abstract_parameters.end(); ++i)
        {
          parameters.push_back(dynamic_reconfigure::ParamDescription(**i));
        }
      }
    };

    typedef boost::shared_ptr<AbstractGroupDescription> AbstractGroupDescriptionPtr;
    typedef boost::shared_ptr<const AbstractGroupDescription> AbstractGroupDescriptionConstPtr;

    // Final keyword added to class because it has virtual methods and inherits
    // from a class with a non-virtual destructor.
    template<class T, class PT>
    class GroupDescription DYNAMIC_RECONFIGURE_FINAL : public AbstractGroupDescription
    {
    public:
      GroupDescription(std::string a_name, std::string a_type, int a_parent, int a_id, bool a_s, T PT::* a_f) : AbstractGroupDescription(a_name, a_type, a_parent, a_id, a_s), field(a_f)
      {
      }

      GroupDescription(const GroupDescription<T, PT>& g): AbstractGroupDescription(g.name, g.type, g.parent, g.id, g.state), field(g.field), groups(g.groups)
      {
        parameters = g.parameters;
        abstract_parameters = g.abstract_parameters;
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, boost::any &cfg) const override
      {
        PT* config = boost::any_cast<PT*>(cfg);
        if(!dynamic_reconfigure::ConfigTools::getGroupState(msg, name, (*config).*field))
          return false;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          if(!(*i)->fromMessage(msg, n))
            return false;
        }

        return true;
      }

      virtual void setInitialState(boost::any &cfg) const override
      {
        PT* config = boost::any_cast<PT*>(cfg);
        T* group = &((*config).*field);
        group->state = state;

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = boost::any(&((*config).*field));
          (*i)->setInitialState(n);
        }

      }

      virtual void updateParams(boost::any &cfg, GazelleSimConfig &top) const override
      {
        PT* config = boost::any_cast<PT*>(cfg);

        T* f = &((*config).*field);
        f->setParams(top, abstract_parameters);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          boost::any n = &((*config).*field);
          (*i)->updateParams(n, top);
        }
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const boost::any &cfg) const override
      {
        const PT config = boost::any_cast<PT>(cfg);
        dynamic_reconfigure::ConfigTools::appendGroup<T>(msg, name, id, parent, config.*field);

        for(std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = groups.begin(); i != groups.end(); ++i)
        {
          (*i)->toMessage(msg, config.*field);
        }
      }

      T PT::* field;
      std::vector<GazelleSimConfig::AbstractGroupDescriptionConstPtr> groups;
    };

class DEFAULT
{
  public:
    DEFAULT()
    {
      state = true;
      name = "Default";
    }

    void setParams(GazelleSimConfig &config, const std::vector<AbstractParamDescriptionConstPtr> params)
    {
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator _i = params.begin(); _i != params.end(); ++_i)
      {
        boost::any val;
        (*_i)->getValue(config, val);

        if("camera_z"==(*_i)->name){camera_z = boost::any_cast<double>(val);}
        if("camera_phi"==(*_i)->name){camera_phi = boost::any_cast<double>(val);}
        if("camera_f"==(*_i)->name){camera_f = boost::any_cast<double>(val);}
        if("camera_border"==(*_i)->name){camera_border = boost::any_cast<bool>(val);}
        if("lidar_active"==(*_i)->name){lidar_active = boost::any_cast<bool>(val);}
        if("lidar_max_dst"==(*_i)->name){lidar_max_dst = boost::any_cast<double>(val);}
        if("lidar_mkr_size"==(*_i)->name){lidar_mkr_size = boost::any_cast<int>(val);}
        if("exterior_walls"==(*_i)->name){exterior_walls = boost::any_cast<bool>(val);}
        if("obstructions"==(*_i)->name){obstructions = boost::any_cast<bool>(val);}
        if("dyn_load_obs"==(*_i)->name){dyn_load_obs = boost::any_cast<bool>(val);}
        if("collision_type"==(*_i)->name){collision_type = boost::any_cast<int>(val);}
        if("reset_sim"==(*_i)->name){reset_sim = boost::any_cast<bool>(val);}
      }
    }

    double camera_z;
double camera_phi;
double camera_f;
bool camera_border;
bool lidar_active;
double lidar_max_dst;
int lidar_mkr_size;
bool exterior_walls;
bool obstructions;
bool dyn_load_obs;
int collision_type;
bool reset_sim;

    bool state;
    std::string name;

    
}groups;



//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      double camera_z;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      double camera_phi;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      double camera_f;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool camera_border;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool lidar_active;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      double lidar_max_dst;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      int lidar_mkr_size;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool exterior_walls;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool obstructions;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool dyn_load_obs;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      int collision_type;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      bool reset_sim;
//#line 231 "/opt/ros/noetic/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"

    bool __fromMessage__(dynamic_reconfigure::Config &msg)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();

      int count = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        if ((*i)->fromMessage(msg, *this))
          count++;

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i ++)
      {
        if ((*i)->id == 0)
        {
          boost::any n = boost::any(this);
          (*i)->updateParams(n, *this);
          (*i)->fromMessage(msg, n);
        }
      }

      if (count != dynamic_reconfigure::ConfigTools::size(msg))
      {
        ROS_ERROR("GazelleSimConfig::__fromMessage__ called with an unexpected parameter.");
        ROS_ERROR("Booleans:");
        for (unsigned int i = 0; i < msg.bools.size(); i++)
          ROS_ERROR("  %s", msg.bools[i].name.c_str());
        ROS_ERROR("Integers:");
        for (unsigned int i = 0; i < msg.ints.size(); i++)
          ROS_ERROR("  %s", msg.ints[i].name.c_str());
        ROS_ERROR("Doubles:");
        for (unsigned int i = 0; i < msg.doubles.size(); i++)
          ROS_ERROR("  %s", msg.doubles[i].name.c_str());
        ROS_ERROR("Strings:");
        for (unsigned int i = 0; i < msg.strs.size(); i++)
          ROS_ERROR("  %s", msg.strs[i].name.c_str());
        // @todo Check that there are no duplicates. Make this error more
        // explicit.
        return false;
      }
      return true;
    }

    // This version of __toMessage__ is used during initialization of
    // statics when __getParamDescriptions__ can't be called yet.
    void __toMessage__(dynamic_reconfigure::Config &msg, const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__, const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__) const
    {
      dynamic_reconfigure::ConfigTools::clear(msg);
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toMessage(msg, *this);

      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        if((*i)->id == 0)
        {
          (*i)->toMessage(msg, *this);
        }
      }
    }

    void __toMessage__(dynamic_reconfigure::Config &msg) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      __toMessage__(msg, __param_descriptions__, __group_descriptions__);
    }

    void __toServer__(const ros::NodeHandle &nh) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->toServer(nh, *this);
    }

    void __fromServer__(const ros::NodeHandle &nh)
    {
      static bool setup=false;

      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->fromServer(nh, *this);

      const std::vector<AbstractGroupDescriptionConstPtr> &__group_descriptions__ = __getGroupDescriptions__();
      for (std::vector<AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); i++){
        if (!setup && (*i)->id == 0) {
          setup = true;
          boost::any n = boost::any(this);
          (*i)->setInitialState(n);
        }
      }
    }

    void __clamp__()
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const GazelleSimConfig &__max__ = __getMax__();
      const GazelleSimConfig &__min__ = __getMin__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->clamp(*this, __max__, __min__);
    }

    uint32_t __level__(const GazelleSimConfig &config) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      uint32_t level = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); ++i)
        (*i)->calcLevel(level, config, *this);
      return level;
    }

    static const dynamic_reconfigure::ConfigDescription &__getDescriptionMessage__();
    static const GazelleSimConfig &__getDefault__();
    static const GazelleSimConfig &__getMax__();
    static const GazelleSimConfig &__getMin__();
    static const std::vector<AbstractParamDescriptionConstPtr> &__getParamDescriptions__();
    static const std::vector<AbstractGroupDescriptionConstPtr> &__getGroupDescriptions__();

  private:
    static const GazelleSimConfigStatics *__get_statics__();
  };

  template <> // Max and min are ignored for strings.
  inline void GazelleSimConfig::ParamDescription<std::string>::clamp(GazelleSimConfig &config, const GazelleSimConfig &max, const GazelleSimConfig &min) const
  {
    (void) config;
    (void) min;
    (void) max;
    return;
  }

  class GazelleSimConfigStatics
  {
    friend class GazelleSimConfig;

    GazelleSimConfigStatics()
    {
GazelleSimConfig::GroupDescription<GazelleSimConfig::DEFAULT, GazelleSimConfig> Default("Default", "", 0, 0, true, &GazelleSimConfig::groups);
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.camera_z = 0.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.camera_z = 2.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.camera_z = 0.5;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_z", "double", 0, "Camera Height", "", &GazelleSimConfig::camera_z)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_z", "double", 0, "Camera Height", "", &GazelleSimConfig::camera_z)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.camera_phi = 0.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.camera_phi = 90.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.camera_phi = 60.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_phi", "double", 0, "Camera Rot Deg", "", &GazelleSimConfig::camera_phi)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_phi", "double", 0, "Camera Rot Deg", "", &GazelleSimConfig::camera_phi)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.camera_f = 0.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.camera_f = 200.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.camera_f = 70.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_f", "double", 0, "Focal Len", "", &GazelleSimConfig::camera_f)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("camera_f", "double", 0, "Focal Len", "", &GazelleSimConfig::camera_f)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.camera_border = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.camera_border = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.camera_border = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("camera_border", "bool", 0, "Cam Border On", "", &GazelleSimConfig::camera_border)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("camera_border", "bool", 0, "Cam Border On", "", &GazelleSimConfig::camera_border)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.lidar_active = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.lidar_active = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.lidar_active = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("lidar_active", "bool", 0, "Lidar On", "", &GazelleSimConfig::lidar_active)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("lidar_active", "bool", 0, "Lidar On", "", &GazelleSimConfig::lidar_active)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.lidar_max_dst = 0.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.lidar_max_dst = 10.0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.lidar_max_dst = 3.5;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("lidar_max_dst", "double", 0, "Lidar Max Dist", "", &GazelleSimConfig::lidar_max_dst)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<double>("lidar_max_dst", "double", 0, "Lidar Max Dist", "", &GazelleSimConfig::lidar_max_dst)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.lidar_mkr_size = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.lidar_mkr_size = 20;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.lidar_mkr_size = 7;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<int>("lidar_mkr_size", "int", 0, "Lidar Mkr Size", "", &GazelleSimConfig::lidar_mkr_size)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<int>("lidar_mkr_size", "int", 0, "Lidar Mkr Size", "", &GazelleSimConfig::lidar_mkr_size)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.exterior_walls = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.exterior_walls = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.exterior_walls = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("exterior_walls", "bool", 0, "Exterior Walls", "", &GazelleSimConfig::exterior_walls)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("exterior_walls", "bool", 0, "Exterior Walls", "", &GazelleSimConfig::exterior_walls)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.obstructions = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.obstructions = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.obstructions = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("obstructions", "bool", 0, "Obstructions", "", &GazelleSimConfig::obstructions)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("obstructions", "bool", 0, "Obstructions", "", &GazelleSimConfig::obstructions)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.dyn_load_obs = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.dyn_load_obs = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.dyn_load_obs = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("dyn_load_obs", "bool", 0, "Dynamic Load Obstructions", "", &GazelleSimConfig::dyn_load_obs)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("dyn_load_obs", "bool", 0, "Dynamic Load Obstructions", "", &GazelleSimConfig::dyn_load_obs)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.collision_type = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.collision_type = 2;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.collision_type = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<int>("collision_type", "int", 0, "Collision", "{'enum': [{'name': 'None', 'type': 'int', 'value': 0, 'srcline': 28, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'No collision check', 'ctype': 'int', 'cconsttype': 'const int'}, {'name': 'Soft', 'type': 'int', 'value': 1, 'srcline': 29, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'Soft collision check', 'ctype': 'int', 'cconsttype': 'const int'}, {'name': 'Hard', 'type': 'int', 'value': 2, 'srcline': 30, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'Hard collision check', 'ctype': 'int', 'cconsttype': 'const int'}], 'enum_description': 'An enum to set collision'}", &GazelleSimConfig::collision_type)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<int>("collision_type", "int", 0, "Collision", "{'enum': [{'name': 'None', 'type': 'int', 'value': 0, 'srcline': 28, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'No collision check', 'ctype': 'int', 'cconsttype': 'const int'}, {'name': 'Soft', 'type': 'int', 'value': 1, 'srcline': 29, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'Soft collision check', 'ctype': 'int', 'cconsttype': 'const int'}, {'name': 'Hard', 'type': 'int', 'value': 2, 'srcline': 30, 'srcfile': '/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg', 'description': 'Hard collision check', 'ctype': 'int', 'cconsttype': 'const int'}], 'enum_description': 'An enum to set collision'}", &GazelleSimConfig::collision_type)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __min__.reset_sim = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __max__.reset_sim = 1;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __default__.reset_sim = 0;
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.abstract_parameters.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("reset_sim", "bool", 0, "Reset Sim", "", &GazelleSimConfig::reset_sim)));
//#line 291 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __param_descriptions__.push_back(GazelleSimConfig::AbstractParamDescriptionConstPtr(new GazelleSimConfig::ParamDescription<bool>("reset_sim", "bool", 0, "Reset Sim", "", &GazelleSimConfig::reset_sim)));
//#line 246 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      Default.convertParams();
//#line 246 "/opt/ros/noetic/lib/python3/dist-packages/dynamic_reconfigure/parameter_generator_catkin.py"
      __group_descriptions__.push_back(GazelleSimConfig::AbstractGroupDescriptionConstPtr(new GazelleSimConfig::GroupDescription<GazelleSimConfig::DEFAULT, GazelleSimConfig>(Default)));
//#line 369 "/opt/ros/noetic/share/dynamic_reconfigure/cmake/../templates/ConfigType.h.template"

      for (std::vector<GazelleSimConfig::AbstractGroupDescriptionConstPtr>::const_iterator i = __group_descriptions__.begin(); i != __group_descriptions__.end(); ++i)
      {
        __description_message__.groups.push_back(**i);
      }
      __max__.__toMessage__(__description_message__.max, __param_descriptions__, __group_descriptions__);
      __min__.__toMessage__(__description_message__.min, __param_descriptions__, __group_descriptions__);
      __default__.__toMessage__(__description_message__.dflt, __param_descriptions__, __group_descriptions__);
    }
    std::vector<GazelleSimConfig::AbstractParamDescriptionConstPtr> __param_descriptions__;
    std::vector<GazelleSimConfig::AbstractGroupDescriptionConstPtr> __group_descriptions__;
    GazelleSimConfig __max__;
    GazelleSimConfig __min__;
    GazelleSimConfig __default__;
    dynamic_reconfigure::ConfigDescription __description_message__;

    static const GazelleSimConfigStatics *get_instance()
    {
      // Split this off in a separate function because I know that
      // instance will get initialized the first time get_instance is
      // called, and I am guaranteeing that get_instance gets called at
      // most once.
      static GazelleSimConfigStatics instance;
      return &instance;
    }
  };

  inline const dynamic_reconfigure::ConfigDescription &GazelleSimConfig::__getDescriptionMessage__()
  {
    return __get_statics__()->__description_message__;
  }

  inline const GazelleSimConfig &GazelleSimConfig::__getDefault__()
  {
    return __get_statics__()->__default__;
  }

  inline const GazelleSimConfig &GazelleSimConfig::__getMax__()
  {
    return __get_statics__()->__max__;
  }

  inline const GazelleSimConfig &GazelleSimConfig::__getMin__()
  {
    return __get_statics__()->__min__;
  }

  inline const std::vector<GazelleSimConfig::AbstractParamDescriptionConstPtr> &GazelleSimConfig::__getParamDescriptions__()
  {
    return __get_statics__()->__param_descriptions__;
  }

  inline const std::vector<GazelleSimConfig::AbstractGroupDescriptionConstPtr> &GazelleSimConfig::__getGroupDescriptions__()
  {
    return __get_statics__()->__group_descriptions__;
  }

  inline const GazelleSimConfigStatics *GazelleSimConfig::__get_statics__()
  {
    const static GazelleSimConfigStatics *statics;

    if (statics) // Common case
      return statics;

    boost::mutex::scoped_lock lock(dynamic_reconfigure::__init_mutex__);

    if (statics) // In case we lost a race.
      return statics;

    statics = GazelleSimConfigStatics::get_instance();

    return statics;
  }

//#line 28 "/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg"
      const int GazelleSim_None = 0;
//#line 29 "/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg"
      const int GazelleSim_Soft = 1;
//#line 30 "/home/lo/igvc_ws/src/gazelle_sim/cfg/GazelleSim.cfg"
      const int GazelleSim_Hard = 2;
}

#undef DYNAMIC_RECONFIGURE_FINAL

#endif // __GAZELLESIMRECONFIGURATOR_H__
