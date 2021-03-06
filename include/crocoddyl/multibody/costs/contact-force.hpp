///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_COSTS_CONTACT_FORCE_HPP_
#define CROCODDYL_MULTIBODY_COSTS_CONTACT_FORCE_HPP_

#include "crocoddyl/multibody/cost-base.hpp"
#include "crocoddyl/multibody/contact-base.hpp"
#include "crocoddyl/multibody/frames.hpp"

namespace crocoddyl {

class CostModelContactForce : public CostModelAbstract {
 public:
  CostModelContactForce(boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation,
                        const FrameForce& fref, const std::size_t& nu);
  CostModelContactForce(boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation,
                        const FrameForce& fref);
  CostModelContactForce(boost::shared_ptr<StateMultibody> state, const FrameForce& fref, const std::size_t& nu);
  CostModelContactForce(boost::shared_ptr<StateMultibody> state, const FrameForce& fref);
  ~CostModelContactForce();

  void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
            const Eigen::Ref<const Eigen::VectorXd>& u);
  void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
                const Eigen::Ref<const Eigen::VectorXd>& u, const bool& recalc = true);
  boost::shared_ptr<CostDataAbstract> createData(pinocchio::Data* const data);

  const FrameForce& get_fref() const;
  void set_fref(const FrameForce& fref);

 protected:
  FrameForce fref_;
};

struct CostDataContactForce : public CostDataAbstract {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  template <typename Model>
  CostDataContactForce(Model* const model, pinocchio::Data* const data)
      : CostDataAbstract(model, data), Arr_Ru(model->get_activation()->get_nr(), model->get_state()->get_nv()) {
    Arr_Ru.fill(0);
  }
  boost::shared_ptr<ContactDataAbstract> contact_;
  Eigen::MatrixXd Arr_Ru;
};

}  // namespace crocoddyl

#endif  // CROCODDYL_MULTIBODY_COSTS_CONTACT_FORCE_HPP_
