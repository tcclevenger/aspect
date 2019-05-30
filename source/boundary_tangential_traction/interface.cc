/*
  Copyright (C) 2011 - 2018 by the authors of the ASPECT code.

  This file is part of ASPECT.

  ASPECT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  ASPECT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ASPECT; see the file LICENSE.  If not see
  <http://www.gnu.org/licenses/>.
*/


#include <aspect/global.h>
#include <aspect/boundary_tangential_traction/interface.h>
#include <aspect/simulator_access.h>

#include <deal.II/base/exceptions.h>
#include <tuple>

#include <list>


namespace aspect
{
  namespace BoundaryTangentialTraction
  {
    template <int dim>
    Interface<dim>::~Interface ()
    {}


    template <int dim>
    void
    Interface<dim>::initialize ()
    {}



    template <int dim>
    void
    Interface<dim>::update ()
    {}

    template <int dim>
    Tensor<1,dim>
    Interface<dim>::boundary_tangential_traction (const types::boundary_id /*boundary_indicator*/,
                                       const Point<dim> &/*position*/,
                                       const Tensor<1,dim> &/*normal_vector*/) const
    {
      // No default implementation, user should define this function
      return Tensor<1,dim>();
    }


    template <int dim>
    void
    Interface<dim>::
    declare_parameters (dealii::ParameterHandler &)
    {}


    template <int dim>
    void
    Interface<dim>::parse_parameters (dealii::ParameterHandler &)
    {}


// -------------------------------- Deal with registering boundary_tangential_traction models and automating
// -------------------------------- their setup and selection at run time

    namespace
    {
      std::tuple
      <void *,
      void *,
      aspect::internal::Plugins::PluginList<Interface<2> >,
      aspect::internal::Plugins::PluginList<Interface<3> > > registered_plugins;
    }



    template <int dim>
    void
    register_boundary_tangential_traction (const std::string &name,
                                const std::string &description,
                                void (*declare_parameters_function) (ParameterHandler &),
                                Interface<dim> *(*factory_function) ())
    {
      std::get<dim>(registered_plugins).register_plugin (name,
                                                         description,
                                                         declare_parameters_function,
                                                         factory_function);
    }


    template <int dim>
    Interface<dim> *
    create_boundary_tangential_traction (const std::string &name)
    {
      Interface<dim> *plugin = std::get<dim>(registered_plugins).create_plugin (name,
                                                                                "Boundary tangential traction conditions");
      return plugin;
    }



    template <int dim>
    std::string
    get_names ()
    {
      return std::get<dim>(registered_plugins).get_pattern_of_names ();
    }



    template <int dim>
    void
    declare_parameters (ParameterHandler &prm)
    {
      std::get<dim>(registered_plugins).declare_parameters (prm);
    }



    template <int dim>
    void
    write_plugin_graph (std::ostream &out)
    {
      std::get<dim>(registered_plugins).write_plugin_graph ("Boundary tangential traction interface",
                                                            out);
    }
  }
}

// explicit instantiations
namespace aspect
{
  namespace internal
  {
    namespace Plugins
    {
      template <>
      std::list<internal::Plugins::PluginList<BoundaryTangentialTraction::Interface<2> >::PluginInfo> *
      internal::Plugins::PluginList<BoundaryTangentialTraction::Interface<2> >::plugins = nullptr;
      template <>
      std::list<internal::Plugins::PluginList<BoundaryTangentialTraction::Interface<3> >::PluginInfo> *
      internal::Plugins::PluginList<BoundaryTangentialTraction::Interface<3> >::plugins = nullptr;
    }
  }

  namespace BoundaryTangentialTraction
  {
#define INSTANTIATE(dim) \
    template class Interface<dim>; \
    \
    template \
    void \
    register_boundary_tangential_traction<dim> (const std::string &, \
                                                const std::string &, \
                                                void ( *) (ParameterHandler &), \
                                                Interface<dim> *( *) ()); \
    \
    template  \
    void \
    declare_parameters<dim> (ParameterHandler &); \
    \
    template \
    void \
    write_plugin_graph<dim> (std::ostream &); \
    \
    template  \
    std::string \
    get_names<dim> (); \
    \
    template \
    Interface<dim> * \
    create_boundary_tangential_traction<dim> (const std::string &);

    ASPECT_INSTANTIATE(INSTANTIATE)
  }
}
