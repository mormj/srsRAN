
#ifndef SCHED_UE_H
#define SCHED_UE_H

#include <map>
#include "srslte/common/log.h"
#include "srslte/interfaces/sched_interface.h"

#include "scheduler_harq.h"

namespace srsenb {

class sched_ue {
  
public: 
  
  // used by sched_metric
  uint32_t ue_idx;   
  
  typedef struct {
    uint32_t cce_start[4][6];
    uint32_t nof_loc[4]; 
  } sched_dci_cce_t;
  
  /*************************************************************
   * 
   * FAPI-like Interface 
   * 
   ************************************************************/
  sched_ue();
  void reset();
  void phy_config_enabled(uint32_t tti, bool enabled);
  void set_cfg(uint16_t rnti, sched_interface::ue_cfg_t* cfg, sched_interface::cell_cfg_t *cell_cfg, 
              srslte_regs_t *regs, srslte::log *log_h);

  void set_bearer_cfg(uint32_t lc_id, srsenb::sched_interface::ue_bearer_cfg_t* cfg);
  void rem_bearer(uint32_t lc_id);
  
  void dl_buffer_state(uint8_t lc_id, uint32_t tx_queue, uint32_t retx_queue);
  void ul_buffer_state(uint8_t lc_id, uint32_t bsr); 
  void ul_phr(int phr); 
  void mac_buffer_state(uint32_t ce_code);
  void ul_recv_len(uint32_t lcid, uint32_t len);
  void set_ul_cqi(uint32_t tti, uint32_t cqi, uint32_t ul_ch_code);
  void set_dl_cqi(uint32_t tti, uint32_t cqi);
  int  set_ack_info(uint32_t tti, bool ack);
  void set_ul_crc(uint32_t tti, bool crc_res);

/*******************************************************
 * Custom functions 
 *******************************************************/

  void tpc_inc(); 
  void tpc_dec();

  void set_max_mcs(int mcs_ul, int mcs_dl); 
  void set_fixed_mcs(int mcs_ul, int mcs_dl); 
  
  
  
/*******************************************************
 * Functions used by scheduler metric objects
 *******************************************************/

  uint32_t   get_required_prb_dl(uint32_t req_bytes, uint32_t nof_ctrl_symbols); 
  uint32_t   get_required_prb_ul(uint32_t req_bytes); 

  uint32_t   get_pending_dl_new_data(uint32_t tti);
  uint32_t   get_pending_ul_new_data(uint32_t tti);

  dl_harq_proc *get_pending_dl_harq(uint32_t tti);
  dl_harq_proc *get_empty_dl_harq();   
  ul_harq_proc *get_ul_harq(uint32_t tti);   

/*******************************************************
 * Functions used by the scheduler object
 *******************************************************/

  void       set_sr();
  void       unset_sr();

  int        generate_format1(dl_harq_proc *h, sched_interface::dl_sched_data_t *data, uint32_t tti, uint32_t cfi);     
  int        generate_format0(ul_harq_proc *h, sched_interface::ul_sched_data_t *data, uint32_t tti, bool cqi_request);     
  
  uint32_t         get_aggr_level(uint32_t nof_bits);
  sched_dci_cce_t *get_locations(uint32_t current_cfi, uint32_t sf_idx);
  
  bool       needs_cqi(uint32_t tti, bool will_send = false); 
  uint32_t   get_max_retx(); 
  
  bool       get_pucch_sched(uint32_t current_tti, uint32_t prb_idx[2], uint32_t *L);
  bool       pucch_sr_collision(uint32_t current_tti, uint32_t n_cce); 
  
private: 
  
  typedef struct {
    sched_interface::ue_bearer_cfg_t cfg; 
    int buf_tx;
    int buf_retx; 
    int bsr;
  } ue_bearer_t; 
  
  bool       is_sr_triggered();
  uint32_t   get_pending_ul_old_data();  
  int        alloc_pdu(int tbs, sched_interface::dl_sched_pdu_t* pdu);  

  static uint32_t format1_count_prb(uint32_t bitmask, uint32_t cell_nof_prb); 
  static int cqi_to_tbs(uint32_t cqi, uint32_t nof_prb, uint32_t nof_re, uint32_t max_mcs, uint32_t *mcs);
  static int alloc_tbs(uint32_t cqi, uint32_t nof_prb, uint32_t nof_re, uint32_t req_bytes, uint32_t max_mcs, int *mcs); 
  
  static bool bearer_is_ul(ue_bearer_t *lch);
  static bool bearer_is_dl(ue_bearer_t *lch);
  
  bool is_first_dl_tx();
      
  
  sched_interface::ue_cfg_t cfg; 
  srslte_cell_t cell; 
  srslte::log* log_h;
  
  /* Buffer states */
  bool sr; 
  int buf_mac;
  int buf_ul; 
  ue_bearer_t lch[sched_interface::MAX_LC];
  
  int      power_headroom; 
  uint32_t dl_cqi;
  uint32_t dl_cqi_tti; 
  uint32_t cqi_request_tti; 
  uint32_t ul_cqi; 
  uint32_t ul_cqi_tti; 
  uint16_t rnti; 
  uint32_t max_mcs_dl; 
  uint32_t max_mcs_ul; 
  int      fixed_mcs_ul; 
  int      fixed_mcs_dl; 

  int next_tpc_pusch;
  int next_tpc_pucch; 

  // Allowed DCI locations per CFI and per subframe    
  sched_dci_cce_t dci_locations[3][10];   

  const static int SCHED_MAX_HARQ_PROC = 8; 
  dl_harq_proc dl_harq[SCHED_MAX_HARQ_PROC]; 
  ul_harq_proc ul_harq[SCHED_MAX_HARQ_PROC]; 
  
  bool phy_config_dedicated_enabled;
    
}; 
  
}
 

#endif 
