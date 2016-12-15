#ifndef _ACI_COMM_UAV_GPS_HPP_
#define _ACI_COMM_UAV_GPS_HPP_
#ifdef __cplusplus

#include "engine.hpp"
#include "aci_comm_uav.hpp"
#include "conversion_lambda.hpp"
#include "uav_commons.hpp"

namespace acc
{
  template<class T, class FloatingPointPrecision>
  class GPSS
  {

  private:

    GPSS(T* uav_ptr) {

    };

  public:
    friend class UAV;

    GPSS() {};

  }; /* class GPSS */

} /* namspace acc */

#endif /* ifdef __cplusplus */
#endif /* define _ACI_COMM_UAV_GPS_HPP_ */