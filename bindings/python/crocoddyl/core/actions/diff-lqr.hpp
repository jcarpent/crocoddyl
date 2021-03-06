///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_CORE_ACTIONS_DIFF_LQR_HPP_
#define BINDINGS_PYTHON_CROCODDYL_CORE_ACTIONS_DIFF_LQR_HPP_

#include "crocoddyl/core/actions/diff-lqr.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

void exposeDifferentialActionLQR() {
  bp::class_<DifferentialActionModelLQR, bp::bases<DifferentialActionModelAbstract> >(
      "DifferentialActionModelLQR",
      "Differential action model for linear dynamics and quadratic cost.\n\n"
      "This class implements a linear dynamics, and quadratic costs (i.e.\n"
      "LQR action). Since the DAM is a second order system, and the integrated\n"
      "action models are implemented as being second order integrators. This\n"
      "class implements a second order linear system given by\n"
      "  x = [q, v]\n"
      "  dv = Fq q + Fv v + Fu u + f0\n"
      "where Fq, Fv, Fu and f0 are randomly chosen constant terms. On the other\n"
      "hand the cost function is given by\n"
      "  l(x,u) = 1/2 [x,u].T [Lxx Lxu; Lxu.T Luu] [x,u] + [lx,lu].T [x,u].",
      bp::init<int, int, bp::optional<bool> >(bp::args(" self", " nq", " nu", " driftFree=True"),
                                              "Initialize the differential LQR action model.\n\n"
                                              ":param nx: dimension of the state vector\n"
                                              ":param nu: dimension of the control vector\n"
                                              ":param driftFree: enable/disable the bias term of the linear dynamics"))
      .def("calc", &DifferentialActionModelLQR::calc_wrap,
           DiffActionModel_calc_wraps(bp::args(" self", " data", " x", " u=None"),
                                      "Compute the next state and cost value.\n\n"
                                      "It describes the time-continuous evolution of the LQR system. Additionally it\n"
                                      "computes the cost value associated to this discrete state and control pair.\n"
                                      ":param data: action data\n"
                                      ":param x: time-continuous state vector\n"
                                      ":param u: time-continuous control input"))
      .def<void (DifferentialActionModelLQR::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                const Eigen::VectorXd&, const Eigen::VectorXd&, const bool&)>(
          "calcDiff", &DifferentialActionModelLQR::calcDiff_wrap,
          bp::args(" self", " data", " x", " u=None", " recalc=True"),
          "Compute the derivatives of the differential LQR dynamics and cost functions.\n\n"
          "It computes the partial derivatives of the differential LQR system and the\n"
          "cost function. If recalc == True, it first updates the state evolution\n"
          "and cost value. This function builds a quadratic approximation of the\n"
          "action model (i.e. dynamical system and cost function).\n"
          ":param data: action data\n"
          ":param x: time-continuous state vector\n"
          ":param u: time-continuous control input\n"
          ":param recalc: If true, it updates the state evolution and the cost value.")
      .def<void (DifferentialActionModelLQR::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                const Eigen::VectorXd&, const Eigen::VectorXd&)>(
          "calcDiff", &DifferentialActionModelLQR::calcDiff_wrap, bp::args(" self", " data", " x", " u"))
      .def<void (DifferentialActionModelLQR::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                const Eigen::VectorXd&)>(
          "calcDiff", &DifferentialActionModelLQR::calcDiff_wrap, bp::args(" self", " data", " x"))
      .def<void (DifferentialActionModelLQR::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                const Eigen::VectorXd&, const bool&)>(
          "calcDiff", &DifferentialActionModelLQR::calcDiff_wrap, bp::args(" self", " data", " x", " recalc"))
      .def("createData", &DifferentialActionModelLQR::createData, bp::args(" self"),
           "Create the differential LQR action data.")
      .add_property(
          "Fq", bp::make_function(&DifferentialActionModelLQR::get_Fq, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Fq, "Jacobian of the dynamics")
      .add_property(
          "Fv", bp::make_function(&DifferentialActionModelLQR::get_Fv, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Fv, "Jacobian of the dynamics")
      .add_property(
          "Fu", bp::make_function(&DifferentialActionModelLQR::get_Fu, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Fu, "Jacobian of the dynamics")
      .add_property(
          "f0", bp::make_function(&DifferentialActionModelLQR::get_f0, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_f0, "dynamics drift")
      .add_property(
          "lx", bp::make_function(&DifferentialActionModelLQR::get_lx, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_lx, "Jacobian of the cost")
      .add_property(
          "lu", bp::make_function(&DifferentialActionModelLQR::get_lu, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_lu, "Jacobian of the cost")
      .add_property(
          "Lxx",
          bp::make_function(&DifferentialActionModelLQR::get_Lxx, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Lxx, "Hessian of the cost")
      .add_property(
          "Lxu",
          bp::make_function(&DifferentialActionModelLQR::get_Lxu, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Lxu, "Hessian of the cost")
      .add_property(
          "Luu",
          bp::make_function(&DifferentialActionModelLQR::get_Luu, bp::return_value_policy<bp::return_by_value>()),
          &DifferentialActionModelLQR::set_Luu, "Hessian of the cost");

  boost::python::register_ptr_to_python<boost::shared_ptr<DifferentialActionDataLQR> >();

  bp::class_<DifferentialActionDataLQR, bp::bases<DifferentialActionDataAbstract> >(
      "DifferentialActionDataLQR", "Action data for the differential LQR system.",
      bp::init<DifferentialActionModelLQR*>(bp::args(" self", " model"),
                                            "Create differential LQR data.\n\n"
                                            ":param model: differential LQR action model"));
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_CORE_ACTIONS_DIFF_LQR_HPP_
