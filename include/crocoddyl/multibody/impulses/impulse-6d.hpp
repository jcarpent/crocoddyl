///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_IMPULSES_IMPULSE_6D_HPP_
#define CROCODDYL_MULTIBODY_IMPULSES_IMPULSE_6D_HPP_

#include "crocoddyl/multibody/impulse-base.hpp"
#include <pinocchio/spatial/motion.hpp>
#include <pinocchio/multibody/data.hpp>

namespace crocoddyl {

template<typename Scalar> struct ImpulseData6DTpl;

template <typename _Scalar>
class ImpulseModel6DTpl : public ImpulseModelAbstractTpl<_Scalar> {
 public:
  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ImpulseModelAbstractTpl<Scalar> Base;
  typedef StateMultibodyTpl<Scalar> StateMultibody;
  typedef ImpulseDataAbstractTpl<Scalar> ImpulseDataAbstract;
  typedef ImpulseData6DTpl<Scalar> ImpulseData6D;
  typedef typename MathBase::Vector2s Vector2s;
  typedef typename MathBase::Vector3s Vector3s;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;
  
  ImpulseModel6DTpl(boost::shared_ptr<StateMultibody> state, const std::size_t& frame);
  ~ImpulseModel6DTpl();

  void calc(const boost::shared_ptr<ImpulseDataAbstract>& data, const Eigen::Ref<const VectorXs>& x);
  void calcDiff(const boost::shared_ptr<ImpulseDataAbstract>& data, const Eigen::Ref<const VectorXs>& x);
  void updateForce(const boost::shared_ptr<ImpulseDataAbstract>& data, const VectorXs& force);
  boost::shared_ptr<ImpulseDataAbstract> createData(pinocchio::DataTpl<Scalar>* const data);

  const std::size_t& get_frame() const;

protected:
  using Base::state_;
  using Base::ni_;
  
 private:
  std::size_t frame_;
};


template <typename _Scalar>
struct ImpulseData6DTpl : public ImpulseDataAbstractTpl<_Scalar> {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ImpulseDataAbstractTpl<Scalar> Base;
  typedef typename MathBase::Vector2s Vector2s;
  typedef typename MathBase::Vector3s Vector3s;
  typedef typename MathBase::Matrix3s Matrix3s;
  typedef typename MathBase::Matrix6xs Matrix6xs;

  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  template <typename Model>
  ImpulseData6DTpl(Model* const model, pinocchio::DataTpl<Scalar>* const data)
    : Base(model, data),
        jMf(model->get_state()->get_pinocchio().frames[model->get_frame()].placement),
        fXj(jMf.inverse().toActionMatrix()),
        fJf(6, model->get_state()->get_nv()),
        v_partial_dq(6, model->get_state()->get_nv()),
        v_partial_dv(6, model->get_state()->get_nv()) {
    frame = model->get_frame();
    joint = model->get_state()->get_pinocchio().frames[frame].parent;
    fJf.fill(0);
    v_partial_dq.fill(0);
    v_partial_dv.fill(0);
  }

  pinocchio::SE3Tpl<Scalar> jMf;
  typename pinocchio::SE3Tpl<Scalar>::ActionMatrixType fXj;
  Matrix6xs fJf;
  Matrix6xs v_partial_dq;
  Matrix6xs v_partial_dv;

  using Base::pinocchio;
  using Base::joint;
  using Base::frame;
  using Base::Jc;
  using Base::dv0_dq;
  using Base::df_dq;
  using Base::f;  
  
};

typedef ImpulseModel6DTpl<double> ImpulseModel6D;
typedef ImpulseData6DTpl<double> ImpulseData6D;
  
}  // namespace crocoddyl


/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/multibody/impulses/impulse-6d.hxx"

#endif  // CROCODDYL_MULTIBODY_IMPULSES_IMPULSE_6D_HPP_
