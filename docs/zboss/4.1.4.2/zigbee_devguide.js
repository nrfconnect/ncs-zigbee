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
    ] ]
];