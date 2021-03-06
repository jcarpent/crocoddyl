///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "crocoddyl/multibody/costs/centroidal-momentum.hpp"
#include <pinocchio/algorithm/centroidal-derivatives.hpp>

namespace crocoddyl {

CostModelCentroidalMomentum::CostModelCentroidalMomentum(boost::shared_ptr<StateMultibody> state,
                                                         boost::shared_ptr<ActivationModelAbstract> activation,
                                                         const Vector6d& mref, const std::size_t& nu)
    : CostModelAbstract(state, activation, nu), mref_(mref) {
  if (activation_->get_nr() != 6) {
    throw std::invalid_argument("nr is equals to 6");
  }
}

CostModelCentroidalMomentum::CostModelCentroidalMomentum(boost::shared_ptr<StateMultibody> state,
                                                         boost::shared_ptr<ActivationModelAbstract> activation,
                                                         const Vector6d& mref)
    : CostModelAbstract(state, activation), mref_(mref) {
  if (activation_->get_nr() != 6) {
    throw std::invalid_argument("nr is equals to 6");
  }
}

CostModelCentroidalMomentum::CostModelCentroidalMomentum(boost::shared_ptr<StateMultibody> state, const Vector6d& mref,
                                                         const std::size_t& nu)
    : CostModelAbstract(state, 6, nu), mref_(mref) {}

CostModelCentroidalMomentum::CostModelCentroidalMomentum(boost::shared_ptr<StateMultibody> state, const Vector6d& mref)
    : CostModelAbstract(state, 6), mref_(mref) {}

CostModelCentroidalMomentum::~CostModelCentroidalMomentum() {}

void CostModelCentroidalMomentum::calc(const boost::shared_ptr<CostDataAbstract>& data,
                                       const Eigen::Ref<const Eigen::VectorXd>&,
                                       const Eigen::Ref<const Eigen::VectorXd>&) {
  // Compute the cost residual give the reference CentroidalMomentum
  data->r = data->pinocchio->hg.toVector() - mref_;

  activation_->calc(data->activation, data->r);
  data->cost = data->activation->a_value;
}

void CostModelCentroidalMomentum::calcDiff(const boost::shared_ptr<CostDataAbstract>& data,
                                           const Eigen::Ref<const Eigen::VectorXd>& x,
                                           const Eigen::Ref<const Eigen::VectorXd>& u, const bool& recalc) {
  if (recalc) {
    calc(data, x, u);
  }

  CostDataCentroidalMomentum* d = static_cast<CostDataCentroidalMomentum*>(data.get());
  const std::size_t& nv = state_->get_nv();
  Eigen::Ref<pinocchio::Data::Matrix6x> Rq = data->Rx.leftCols(nv);
  Eigen::Ref<pinocchio::Data::Matrix6x> Rv = data->Rx.rightCols(nv);

  activation_->calcDiff(data->activation, data->r, recalc);
  pinocchio::getCentroidalDynamicsDerivatives(state_->get_pinocchio(), *data->pinocchio, Rq, d->hdot_partial_dq,
                                              d->hdot_partial_dv, Rv);

  // The derivative computation in pinocchio does not take the frame of reference into
  // account. So we need to update the com frame as well.
  for (int i = 0; i < data->pinocchio->Jcom.cols(); ++i) {
    data->Rx.block<3, 1>(3, i) -= data->pinocchio->Jcom.col(i).cross(data->pinocchio->hg.linear());
  }

  d->Arr_Rx.noalias() = data->activation->Arr * data->Rx;
  data->Lx.noalias() = data->Rx.transpose() * data->activation->Ar;
  data->Lxx.noalias() = data->Rx.transpose() * d->Arr_Rx;
}

boost::shared_ptr<CostDataAbstract> CostModelCentroidalMomentum::createData(pinocchio::Data* const data) {
  return boost::make_shared<CostDataCentroidalMomentum>(this, data);
}

const Vector6d& CostModelCentroidalMomentum::get_mref() const { return mref_; }

void CostModelCentroidalMomentum::set_mref(const Vector6d& mref_in) { mref_ = mref_in; }

}  // namespace crocoddyl
