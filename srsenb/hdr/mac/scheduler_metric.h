#ifndef SCHED_METRIC_H
#define SCHED_METRIC_H

#include "mac/scheduler.h"

namespace srsenb {
  
class dl_metric_rr : public sched::metric_dl
{
public:
  void            new_tti(std::map<uint16_t,sched_ue> &ue_db, uint32_t start_rb, uint32_t nof_rb, uint32_t nof_ctrl_symbols, uint32_t tti);
  dl_harq_proc*   get_user_allocation(sched_ue *user); 
private:
  
  const static int MAX_RBG = 25; 
  
  bool new_allocation(uint32_t nof_rbg, uint32_t* rbgmask); 
  void update_allocation(uint32_t new_mask); 
  bool allocation_is_valid(uint32_t mask); 
  
  
  uint32_t get_required_rbg(sched_ue *user, uint32_t tti); 
  uint32_t count_rbg(uint32_t mask); 
  uint32_t calc_rbg_mask(bool mask[25]); 
  
  bool used_rb[MAX_RBG]; 

  uint32_t nof_users_with_data; 
  
  uint32_t current_tti; 
  uint32_t total_rb;
  uint32_t used_rb_mask;
  uint32_t nof_ctrl_symbols;
  uint32_t available_rb;
};

class ul_metric_rr : public sched::metric_ul
{
public:
  void           new_tti(std::map<uint16_t,sched_ue> &ue_db, uint32_t nof_rb, uint32_t tti);
  ul_harq_proc*  get_user_allocation(sched_ue *user); 
  void           update_allocation(ul_harq_proc::ul_alloc_t alloc); 
private:
  
  const static int MAX_PRB = 100; 
  
  bool new_allocation(uint32_t L, ul_harq_proc::ul_alloc_t *alloc);
  bool allocation_is_valid(ul_harq_proc::ul_alloc_t alloc); 

  uint32_t nof_users_with_data; 

  bool used_rb[MAX_PRB]; 
  uint32_t current_tti; 
  uint32_t nof_rb; 
  uint32_t available_rb;
};

  
}

#endif

