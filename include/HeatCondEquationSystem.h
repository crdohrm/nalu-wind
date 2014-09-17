/*------------------------------------------------------------------------*/
/*  Nalu 1.0 Copyright 2014 Sandia Corporation.                           */
/*  This software is released under the BSD license detailed              */
/*  in the file, LICENSE which is located in the top-level Nalu           */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/


#ifndef HeatCondEquationSystem_h
#define HeatCondEquationSystem_h

#include <EquationSystem.h>
#include <FieldTypeDef.h>
#include <NaluParsing.h>

#include <stk_mesh/base/FieldBase.hpp>
#include <stk_mesh/base/CoordinateSystems.hpp>

namespace stk{
struct topology;
}

namespace sierra{
namespace nalu{

class Realm;
class AssembleNodalGradAlgorithmDriver;
class EquationSystems;

class HeatCondEquationSystem : public EquationSystem {

public:

  HeatCondEquationSystem(
    EquationSystems& equationSystems);
  virtual ~HeatCondEquationSystem();

  void register_nodal_fields(
    stk::mesh::Part *part);

  void register_edge_fields(
    stk::mesh::Part *part);

  void register_element_fields(
    stk::mesh::Part *part,
    const stk::topology &theTopo);

  void register_interior_algorithm(
    stk::mesh::Part *part);

  void register_wall_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo,
    const WallBoundaryConditionData &wallBCData);
  
  virtual void register_contact_bc(
    stk::mesh::Part *part,
    const stk::topology &theTopo,
    const ContactBoundaryConditionData &contactBCData);

  virtual void register_initial_condition_fcn(
      stk::mesh::Part *part,
      const std::map<std::string, std::string> &theNames,
      const std::map<std::string, std::vector<double> > &theParams);

  void solve_and_update();

  void initialize();
  void reinitialize_linear_system();
 
  void predict_state();
  
  virtual void load(const YAML::Node & node)
  {
    EquationSystem::load(node);
    get_if_present(node, "use_collocation", collocationForViscousTerms_, false);
  }


  ScalarFieldType *temperature_;
  VectorFieldType *dtdx_;
  ScalarFieldType *tTmp_;
  ScalarFieldType *dualNodalVolume_;
  VectorFieldType *coordinates_;
  ScalarFieldType *exact_temperature_;
  VectorFieldType *exact_dtdx_;
  VectorFieldType *exact_laplacian_;
  
  ScalarFieldType *density_;
  ScalarFieldType *specHeat_;
  ScalarFieldType *thermalCond_;

  GenericFieldType *scVolume_;
  VectorFieldType *edgeAreaVec_;
 
  AssembleNodalGradAlgorithmDriver *assembleNodalGradAlgDriver_;
  bool isInit_;
  bool collocationForViscousTerms_;
  
};

} // namespace nalu
} // namespace Sierra

#endif