
#ifndef _aspect_timer_output_h
#define _aspect_timer_output_h


#include <deal.II/base/config.h>
#include <deal.II/base/exceptions.h>
#include <deal.II/base/mpi.h>
#include <deal.II/base/timer.h>
#include <deal.II/base/utilities.h>

#include <chrono>
#include <list>
#include <map>
#include <string>

#include <fstream>




namespace aspect
{
  using namespace dealii;

  class MyTimerOutput
  {
    public:
      MyTimerOutput (const MPI_Comm               mpi_comm,
                     const unsigned int           n_timings);

      MyTimerOutput (const MPI_Comm               mpi_comm,
                     const unsigned int           n_timings,
                     const bool                   ignore_first);

      void initialize_sections ();

      void enter_subsection (const std::string &section_name);
      void leave_subsection (const std::string &section_name = std::string());

      std::map<std::string, std::vector<double> > get_all_times () const;
      std::map<std::string, std::vector<double> > get_min_avg_max () const;

      void print_data_screen () const;
      void print_data_file (const std::string  &filename_and_path,
                            const std::string  &problem_type,
                            const std::string  &refinement_step,
                            const unsigned int cells,
                            const types::global_dof_index dofs,
                            const unsigned int procs,
                            const unsigned int gmres_iterations,
                            const double workload_imbalance);

      void reset ();

    private:
      Timer              timer;

      struct Section
      {
        Timer  timer;
        double total_wall_time;
        unsigned int n_calls;
        std::vector<double> time_vec;
      };

      std::map<std::string, Section> sections;

      std::list<std::string> active_sections;

      MPI_Comm            mpi_communicator;
      unsigned int n_timings;
      bool ignore_first;
  };

}




#endif








