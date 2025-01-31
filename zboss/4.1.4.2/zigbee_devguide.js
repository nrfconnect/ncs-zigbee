var zigbee_devguide =
[
    [ "ZBOSS release notes", "zboss_release_notes.html", [
      [ "ZBOSS 4.1.4.2 version dated 06/28/2024", "zboss_release_notes.html#zboss_zoi_release_4_1_4_2", null ],
      [ "ZBOSS 4.1.4.1 version dated 03/15/2024", "zboss_release_notes.html#zboss_zoi_release_4_1_4_1", null ]
    ] ],
    [ "Zigbee stack features", "zigbee_features.html", [
      [ "Supported Zigbee standards", "zigbee_features.html#supported_zigbee_standards", null ],
      [ "Implemented features", "zigbee_features.html#implemented_features", null ]
    ] ],
    [ "Programming principles", "zigbee_prog_principles.html", [
      [ "Application structure", "zigbee_prog_principles.html#zigbee_app_structure", [
        [ "Zigbee type definitions", "zigbee_prog_principles.html#zigbee_types", null ],
        [ "Zigbee stack header files", "zigbee_prog_principles.html#zigbee_sdk_h_files", null ],
        [ "Zigbee stack multitasking (scheduler)", "zigbee_prog_principles.html#zigbee_multitasking", [
          [ "Callbacks", "zigbee_prog_principles.html#zigbee_multitasking_callback", null ],
          [ "Timer alarms", "zigbee_prog_principles.html#zigbee_multitasking_alarms", null ],
          [ "Scheduler API", "zigbee_prog_principles.html#zigbee_multitasking_api", null ]
        ] ],
        [ "Zigbee stack memory management subsystem", "zigbee_prog_principles.html#zigbee_stack_memory_mgmt", null ],
        [ "Time subsystem", "zigbee_prog_principles.html#zigbee_time", null ]
      ] ],
      [ "Debugging", "zigbee_prog_principles.html#zigbee_debugging", null ],
      [ "Power saving feature", "zigbee_prog_principles.html#zigbee_power_optimization", [
        [ "Configuring sleepy behavior for end devices", "zigbee_prog_principles.html#zigbee_power_optimization_sleepy", null ]
      ] ],
      [ "End device keep alive", "zigbee_prog_principles.html#zigbee_keep_alive_mechanism", null ],
      [ "Data polling mechanism", "zigbee_prog_principles.html#zigbee_data_polling_mechanism", [
        [ "Turbo poll", "zigbee_prog_principles.html#zigbee_data_polling_mechanism_turbo", null ]
      ] ],
      [ "Production configuration", "zigbee_prog_principles.html#zigbee_production_config", null ]
    ] ],
    [ "Zigbee stack API overview", "using_zigbee__z_c_l.html", [
      [ "Common ZCL terms and definitions", "using_zigbee__z_c_l.html#ZCL_definitions", null ],
      [ "Implementing a Zigbee end product with ZCL", "using_zigbee__z_c_l.html#zigbee_device_imp", [
        [ "Declaring attributes", "using_zigbee__z_c_l.html#att_declaration", null ],
        [ "Declaring cluster", "using_zigbee__z_c_l.html#cluster_declaration", [
          [ "Declaring custom cluster", "using_zigbee__z_c_l.html#cluster_declaration_custom", null ]
        ] ],
        [ "Declaring cluster list", "using_zigbee__z_c_l.html#cluster_list_declaration", null ],
        [ "Declaring endpoint", "using_zigbee__z_c_l.html#endpoint_dec", null ],
        [ "Declaring simple descriptor", "using_zigbee__z_c_l.html#simple_desc_declaration", null ],
        [ "Declaring Zigbee device context", "using_zigbee__z_c_l.html#zigbee_device_context_dec", null ],
        [ "Declaring Zigbee device context with multiple endpoints", "using_zigbee__z_c_l.html#zigbee_device_context_mult_ep_dec", null ],
        [ "Registering device context", "using_zigbee__z_c_l.html#register_zigbee_device", null ],
        [ "Configuring default ZCL command handler override", "using_zigbee__z_c_l.html#process_zcl_cmd", [
          [ "Executing interception mechanism", "using_zigbee__z_c_l.html#process_zcl_cmd_executing", null ],
          [ "Implementing algorithm for overriding the handling of ZCL commands", "using_zigbee__z_c_l.html#process_zcl_cmd_implementing", null ]
        ] ],
        [ "Implementing Zigbee device callback", "using_zigbee__z_c_l.html#zigbee_device_callback", null ],
        [ "Sending ZCL commands", "using_zigbee__z_c_l.html#zcl_cmds_send", null ],
        [ "Parsing ZCL commands", "using_zigbee__z_c_l.html#zcl_cmd_parse", null ]
      ] ],
      [ "Support for Zigbee commissioning", "using_zigbee__z_c_l.html#zcl_zigbee_commissioning", [
        [ "Commissioning configuration sequence", "using_zigbee__z_c_l.html#zcl_zigbee_commissioning_config_seq", null ],
        [ "Network configuration settings (general)", "using_zigbee__z_c_l.html#network_config_general", null ],
        [ "Network configuration settings (BDB-specific)", "using_zigbee__z_c_l.html#network_config_bdb", null ],
        [ "Stack commissioning start sequence", "using_zigbee__z_c_l.html#stack_start_initiation", null ],
        [ "Zigbee commissioning event handling", "using_zigbee__z_c_l.html#commissioning_event_handling", null ],
        [ "BDB Commissioning API", "using_zigbee__z_c_l.html#bdb_commissioning_api", null ],
        [ "Resetting to factory defaults", "using_zigbee__z_c_l.html#reset_factory_defaults", [
          [ "Resetting to factory defaults externally", "using_zigbee__z_c_l.html#reset_factory_defaults_ext", null ],
          [ "Resetting to factory defaults with a local action", "using_zigbee__z_c_l.html#reset_to_factory_defaults_via_local_action", null ]
        ] ]
      ] ],
      [ "Security", "using_zigbee__z_c_l.html#using_ZCL_security", [
        [ "Zigbee network security API", "using_zigbee__z_c_l.html#security_nwk_security_api", null ],
        [ "Zigbee API for code security installation", "using_zigbee__z_c_l.html#security_nwk_code_security_api", null ]
      ] ]
    ] ],
    [ "Memory configuration feature", "zigbee_mem_cfg_ram_feature.html", "zigbee_mem_cfg_ram_feature" ],
    [ "ZBOSS Zigbee Cluster Library 8 developer guide", "zcl8_guide.html", [
      [ "1. ZCL8 overview", "zcl8_guide.html#zcl8_overview", [
        [ "1.1 List of main cluster changes in ZCL8 specification", "zcl8_guide.html#zcl8_main_changes", null ],
        [ "1.2 ZCL Commands statuses changes introduced in ZCL8 specification", "zcl8_guide.html#zcl8_status_changes", null ]
      ] ],
      [ "2. ZBOSS ZCL8 design", "zcl8_guide.html#zcl8_design", [
        [ "2.1 Compatibility with previous ZCL specifications", "zcl8_guide.html#zcl8_compatibility", null ],
        [ "2.2 Cluster revision", "zcl8_guide.html#zcl8_cluster_revision", [
          [ "2.2.1 How to use cluster revision", "zcl8_guide.html#zcl8_cluster_revision_how_to", null ]
        ] ],
        [ "2.3 Backward compatibility modes", "zcl8_guide.html#zcl8_compatibility_modes", [
          [ "2.3.1 Legacy mode", "zcl8_guide.html#zcl8_comp_legacy_mode", null ],
          [ "2.3.2 Auto mode", "zcl8_guide.html#zcl8_comp_auto_mode", null ],
          [ "2.3.3 Compatibility mode", "zcl8_guide.html#zcl8_comp_compatibility_mode", null ],
          [ "2.3.4 How to use backward compatibility modes", "zcl8_guide.html#zcl8_comp_modes_how_to", null ]
        ] ],
        [ "2.4 How ZBOSS ZCL8 treats ZCL statuses", "zcl8_guide.html#zcl8_status_treatment", null ]
      ] ],
      [ "3. ZBOSS ZCL8 API", "zcl8_guide.html#zc8_api", null ],
      [ "4. References", "zcl8_guide.html#zcl8_references", null ]
    ] ],
    [ "ZBOSS Zigbee 2023 (r23) features", "r23_new_features.html", [
      [ "New features in ZBOSS r23 codebase", "r23_new_features.html#r23_codebase_features", null ],
      [ "r23 stack certified Platform vs certified Product status", "r23_new_features.html#r23_certification", null ],
      [ "Short list of new r23 features", "r23_new_features.html#r23_brief", [
        [ "New r23 Security features", "r23_new_features.html#r23_brief_secur", [
          [ "Key establishment and related features", "r23_new_features.html#r23_key_est", null ],
          [ "ZB 3.0/SE coexistence: initial join & key update methods info", "r23_new_features.html#r23_30_se", null ],
          [ "Trust Center swapout", "r23_new_features.html#r23_tcsw", null ]
        ] ],
        [ "All Hubs / WWAH", "r23_new_features.html#r23_allhubs_brief", [
          [ "Closing holes in r22 security (r22.2 and beyond)", "r23_new_features.html#r22_holes_close", null ],
          [ "Restricted mode.", "r23_new_features.html#r23_restricted_mode", null ]
        ] ],
        [ "APS fragmentation", "r23_new_features.html#r23_apsfrag", null ],
        [ "APS ACK req in APS commands", "r23_new_features.html#r23_apsack", null ],
        [ "Runtime switch of r23 ZBOSS into r22 mode", "r23_new_features.html#r23_r22_switch", null ],
        [ "Extension for group tx in apsde-data.req", "r23_new_features.html#r23_group_tx_brief", null ]
      ] ],
      [ "ZBOSS r23 Details and ZBOSS API description", "r23_new_features.html#r23_details", [
        [ "DLK", "r23_new_features.html#r23_dlk", [
          [ "Why DLK improves Zigbee security", "r23_new_features.html#r23_whydlk", null ],
          [ "Initial starting DLK", "r23_new_features.html#r23_startdlk", null ],
          [ "Force TCLK update", "r23_new_features.html#r23_force_tclkup", null ],
          [ "Device DLK capabilities", "r23_new_features.html#r23_dlk_cap", null ],
          [ "Key negotiation methods", "r23_new_features.html#r23_keyneg_method", null ],
          [ "PSK secrets", "r23_new_features.html#r23_psk_secret", null ],
          [ "Authentication token", "r23_new_features.html#r23_auth_tok", null ],
          [ "Installcode and passcode PSK", "r23_new_features.html#r23_ic_passc", null ],
          [ "API to enable/disable PSK secrets", "r23_new_features.html#r23_ena_psk_secret", null ]
        ] ],
        [ "Device interview", "r23_new_features.html#r23_dev_interview", [
          [ "TC API", "r23_new_features.html#r23_dev_interview_tc", null ],
          [ "Joiner API", "r23_new_features.html#r23_dev_interview_joiner", null ]
        ] ],
        [ "APS frame counter synchronization", "r23_new_features.html#r23_aps_cnt_sync", null ],
        [ "ZB 3.0/SE: get authentication level", "r23_new_features.html#r23_z3se", null ],
        [ "Trust Center swapout", "r23_new_features.html#r23_tc_sw", [
          [ "TC API", "r23_new_features.html#r23_tcsw_api", null ],
          [ "API for ZR and ZED", "r23_new_features.html#r23_tcsw_zrzed", null ]
        ] ],
        [ "PANID conflict resolution API", "r23_new_features.html#r23_panid_conflict", null ]
      ] ],
      [ "All Hubs, WWAH and related r23 features", "r23_new_features.html#r23_allhubs", [
        [ "WWAH / All Hubs features related to security", "r23_new_features.html#r23_allhub_secur", [
          [ "All Hubs Restricted vs WWAH Configuration mode.", "r23_new_features.html#r23_allhub_restr", null ],
          [ "nwkLeaveRequestAllowed setting", "r23_new_features.html#r23_leavereq", null ]
        ] ],
        [ "requireLinkKeyEncryptionForApsTransportKey", "r23_new_features.html#r23_linkkeyencr", [
          [ "Disabling channel and/or pan id change", "r23_new_features.html#r23_disable_panid_change", null ],
          [ "PANID and channel change by TC", "r23_new_features.html#r23_panid_ch", null ]
        ] ],
        [ "Other All Hubs features", "r23_new_features.html#r23_allhubs_other", [
          [ "ZDO decommission req", "r23_new_features.html#r23_decommis", null ],
          [ "ZDO Clear All Bindings req", "r23_new_features.html#r23_clearallbind", null ],
          [ "Parent selection: hub connectivity, preferred parent, long uptime bits", "r23_new_features.html#r23_parentsel", null ],
          [ "Beacon survey", "r23_new_features.html#r23_beacon_surv", null ]
        ] ],
        [ "WWAH features not covered by r23", "r23_new_features.html#wwah_not_r23", null ],
        [ "Runtime switch of r23 ZBOSS into r22 mode", "r23_new_features.html#r23_r23_switch", null ],
        [ "Group TX in apsde-data.req", "r23_new_features.html#r23_group_tx", null ],
        [ "Other ZBOSS API introduced in r23 codebase", "r23_new_features.html#r23_other_api", [
          [ "New signals", "r23_new_features.html#r23_sig", null ]
        ] ],
        [ "The chain of signal handlers", "r23_new_features.html#r23_sig_hnd_chain", null ],
        [ "New debug API for key broadcastiung", "r23_new_features.html#r23_key_br", null ],
        [ "Stack internals update", "r23_new_features.html#r23_int", [
          [ "LQA instead of LQI", "r23_new_features.html#r23_lqa", null ],
          [ "Discovery table instead of extended neighbor", "r23_new_features.html#r23_disc_tbl", null ],
          [ "NVRAM structures change", "r23_new_features.html#r23_nvram", null ]
        ] ]
      ] ],
      [ "Sniffer for r23", "r23_new_features.html#r23_sniff", null ]
    ] ]
];