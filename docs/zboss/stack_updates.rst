.. _zboss_stack_updates:

ZBOSS stack updates
###################

.. contents::
   :local:
   :depth: 2

This page summarizes issue-level fixes and enhancements in the precompiled ZBOSS libraries that ship with the |addon| for the |NCS|.
Each section lists changes for a specific ZBOSS stack version bundled with an |addon| release.

See also the `ZBOSS R23 changelog`_ and the `ZBOSS API documentation`_ for complete upstream documentation.

.. _zboss_stack_updates_4271:

ZBOSS 4.2.7.1
*************

The |addon| |release| release ships ZBOSS stack v4.2.7.1.
The following table lists changes since v4.2.2.5

.. list-table::
   :header-rows: 1
   :widths: 12 68 20

   * - Key
     - Summary
     - Severity
   * - ZBS-490
     - Improve processing of error code in Default Response for Poll Control cluster
     - Enhancement
   * - ZBS-491
     - Improve processing of ZB_ZCL_ATTR_TEMP_MEASUREMENT_TOLERANCE_ID attribute for Temperature measurement cluster
     - Enhancement
   * - ZBS-496
     - Improve processing of ZB_ZCL_SUBGHZ_SRV_CMD_SUSPEND_ZCL_MESSAGES attribute for SubGHz cluster
     - Enhancement
   * - ZBS-501
     - Improve type-checking and add user callback for ZB_ZCL_CMD_WRITE_ATTRIB_RESP
     - Enhancement
   * - ZBS-1813
     - Clarify usage of zboss_start_no_autostart in the API Reference guide
     - Enhancement
   * - ZBS-1980
     - Rename macsplit in Doxygen blocks in sourcecode (R23 codebase) to MAC-Split
     - Enhancement
   * - ZBS-2045
     - Improve details on ZCL device callback input parameters documentation
     - Enhancement
   * - ZBS-2069
     - Back port of PTA configuration API
     - Enhancement
   * - ZBS-2092
     - Fix ZGPD hang during commissioning if GPDF Success transmission fails
     - Major
   * - ZBS-2097
     - Add a callback to notify application after leave for each endpoint
     - Enhancement
   * - ZBS-2165
     - Fix OTA max block size if Source routing is in use
     - Normal
   * - ZBS-2166
     - Return lost Secur Utility lib build target back
     - Minor
   * - ZBS-2203
     - Fix corruption in a list of invisible short addresses caused by MAC_REMOVE_INVISIBLE_SHORT
     - Minor
   * - ZBS-2297
     - Fix using valid zero value as default empty value for address references initialization
     - Major
   * - ZBS-2298
     - Add sending Move to Closest Frequency command of Level Control cluster
     - Enhancement
   * - ZBS-2312
     - Fix missing Physical Contact value of OccupancySensorType attribute in Occupancy Sensor cluster
     - Minor
   * - ZBS-2324
     - Fix uninitialized local variable usage in certification test TP_BDB_FB-CDP-TC-01
     - Minor
   * - ZBS-2333
     - Fix incorrect Extended destination address in Rejoin Response command
     - Normal
   * - ZBS-2336
     - Fix support for /dev/gpiochip on platform NSNG
     - Normal
   * - ZBS-2337
     - Fix incorrect status for invalid fragmentation parameters in zb_apsde_data_confirm
     - Major
   * - ZBS-2343
     - Fix incorrect iteration over ZED neighbors while preparing Link Status command
     - Normal
   * - ZBS-2345
     - Fix possible overflow during calculation of time_to_expire in zb_mcps_poll_indication
     - Normal
   * - ZBS-2360
     - Fix incoming Route Request acceptance criteria according to r23
     - Normal
   * - ZBS-2369
     - Remove depth from trace in zb_cli_utility
     - Enhancement
   * - ZBS-2371
     - Update OpenECC copyright info on devtools create_multiimage
     - Enhancement
   * - ZBS-2373
     - Fix usage of random generating functions
     - Normal
   * - ZBS-2376
     - Restore trace over mux (regression after merge)
     - Minor
   * - ZBS-2388
     - Remove release notes file from ZBOSS R23 codebase
     - Enhancement
   * - ZBS-2391
     - Update a comment for block_num field in zb_apsde_data_req_t
     - Enhancement
   * - ZBS-2394
     - Fix missing APS header in fragmented packet confirm
     - Major
   * - ZBS-2405
     - Fix clearing many-to-one flag when receiving RREQ without MTO for existing route entry
     - Normal
   * - ZBS-2406
     - Add buffer leak protection with automatic buffer freeing for APS user payload packets without callback set
     - Enhancement
   * - ZBS-2407
     - Fix starting route discovery by ZR while not being authenticated
     - Major
   * - ZBS-2408
     - Fix cleared address access in indirect queue purge confirm
     - Major
   * - ZBS-2412
     - Fix incorrect NWK_COST_TO_LQI macro returning wrong value for cost 1
     - Minor
   * - ZBS-2413
     - Do not remove active route when route discovery fails for the same destination
     - Enhancement
   * - ZBS-2414
     - Fix apscInterframeDelay according to Zigbee R23 Core Spec as it is not supposed to be used when Window Size is 1
     - Minor
   * - ZBS-2418
     - Fix releasing APS retrans entries for relayed frame in case of nlde-data.confirm with non-valid status
     - Major
   * - ZBS-2422
     - Fix inconsistency in define naming for Sink helpers header file
     - Minor
   * - ZBS-2423
     - Fix warnings related to unused variables
     - Enhancement
   * - ZBS-2424
     - Fix failed 2nd attempt to join a network via hop when commissioning was interrupted after receiving DLK
     - Major
   * - ZBS-2428
     - Ignore NWK address request with the Extended Response bit set received on ZED
     - Enhancement
   * - ZBS-2443
     - Fix incorrect order of LQI and RSSI values in MAC dump
     - Minor
   * - ZBS-2444
     - Update zgpd_secur_unidir_onoff sample
     - Enhancement
   * - ZBS-2450
     - Fix incorrect comparison of address references
     - Major
   * - ZBS-2454
     - Fix internal zb_nlme_permit_joining_request API location and description
     - Minor
   * - ZBS-2458
     - Fix RSSI field format in dump_converter
     - Minor
   * - ZBS-2465
     - Fix cleaning parent handler after failed commissioning attempt
     - Major
   * - ZBS-2467
     - Fix NCP host application rejoining and restarting from NVRAM for ZED and ZR roles
     - Normal
   * - ZBS-2481
     - Update HMAC-SHA256 function description about supported key length
     - Enhancement
   * - ZBS-2483
     - Fix buffer leak in a confirm for Security Challenge Response
     - Normal
   * - ZBS-2489
     - Fix sending mesh Route Reply using source routing if the device presents in source routing table
     - Normal
   * - ZBS-2495
     - Fix radio enabled state after MAC scan for devices with rx_on_when_idle set to FALSE
     - Normal
   * - ZBS-2497
     - Fix time overflow while scheduling ED Timeout Request
     - Major
   * - ZBS-2498
     - Fix processing MCPS purge confirm with failed status resulting rejoin context leakage
     - Normal
   * - ZBS-2504
     - Fix processing a beacon sent from incorrect device
     - Normal
   * - ZBS-2507
     - Fix buffer data usage after its releasing in zb_mlme_beacon_notify_indication
     - Minor
   * - ZBS-2508
     - Fix a race between sending Beacon Request and switching a channel during Active Scan with a minimal timeout
     - Major
   * - ZBS-2509
     - Fix a race during NLME Permit Join processing while previous NLME Permit Join operation is still in progress
     - Major
   * - ZBS-2516
     - Remove unnecessary delay in releasing APS fragmented transmission resources
     - Enhancement
   * - ZBS-2526
     - Fix potential jitter overflow in OTA Image Notify command handler
     - Minor
   * - ZBS-2528
     - Fix ZBOSS debug build compilation error with ZB_DEBUG_BUFFERS_EXT enabled
     - Minor
   * - ZBS-2530
     - Implement NCP TX power provider functionality
     - Enhancement
   * - ZBS-2534
     - Fix incorrect NWK key generation for key indexes greater than 0
     - Major
   * - ZBS-2535
     - Fix released buffer usage caused by double call of aps_in_frag_stop_entry
     - Major
   * - ZBS-2537
     - Fix user callback invocation with incorrect status for broadcast NWK Update Notify
     - Normal
   * - ZBS-2538
     - Fix address locks leakage on failed commissioning or rejoin attempt
     - Major
   * - ZBS-2545
     - Fix incorrect interpretation of status RET_EXIT for fragmented APS packet transmission
     - Minor
   * - ZBS-2549
     - Remove delay before next transmission attempt in zb_check_cmd_tx_status for ZB_TRANS_CHECK_CHANNEL_BUSY_ERROR case
     - Enhancement
   * - ZBS-2556
     - Prepare documentation describing r22 mode for R23 (R23.2)
     - Enhancement
   * - ZBS-2557
     - Fix sending of queued fragmented APS transmissions when out_frag_q queue is full
     - Major
   * - ZBS-2565
     - Fix corrupted ZCL header in zb_zcl_read_attr_resp_handler
     - Major
   * - ZBS-2567
     - Add a feature for disabling ZDO Diagnostics activity
     - Enhancement
   * - ZBS-2568
     - Fix route expiration on intermediate router hops
     - Normal
   * - ZBS-2569
     - Update win_com_dump parser to support field width specifier
     - Enhancement
   * - ZBS-2580
     - Fix compiler warning regarding unused variable
     - Minor
   * - ZBS-2583
     - Fix NCP RX OSIF buffer serial transport issues
     - Normal
   * - ZBS-2597
     - Fix incorrect neighbor table clear preventing join attempts to other discovered networks
     - Normal
   * - ZBS-2610
     - Fix incorrect nwk_broadcast_delivery_time_passed clearing if a device commissioning is delayed
     - Normal
   * - ZBS-2618
     - Fix a singleton attribute check in endpoints when setting the attribute
     - Major
   * - ZBS-2619
     - Fix discovery table incorrectly cleared during device network join
     - Normal
   * - ZBS-2620
     - Fix not disabled trace after ZB_SET_TRACE_OFF macro call
     - Minor
   * - ZBS-2622
     - Fix access race to ecdhe context if authentication fails due timeout expiration
     - Major
   * - ZBS-2648
     - Fix address table free size calculation to exclude unlocked entries
     - Normal
   * - ZBS-2649
     - Fix wrong device type after failing join attempt
     - Normal
   * - ZBS-2651
     - Fix clearing nwkDiscoveryTable on discovery failures
     - Normal
   * - ZBS-2657
     - Refresh apsFragmentationCache clock field of a device entry on sending request to the device
     - Enhancement
   * - ZBS-2670
     - Fix ZR Proxy not forwarding GP packets from ZGPD to ZC
     - Critical
   * - ZBS-2672
     - Fix diagnostic lib compilation when ZB_CERTIFICATION_HACKS is not defined
     - Normal
   * - ZBS-2702
     - Fix requesting TCLK if a device didn't complete TCLK procedure and rebooted with missing TCLK
     - Normal
   * - ZBS-2704
     - Send a Network Status back to the sender if it is not possible to forward a frame
     - Enhancement
   * - ZBS-2705
     - Don't generate an address conflict for broadcast packet containing our short and long addresses
     - Enhancement
   * - ZBS-2709
     - Fix missing TCLK update after reboot caused by incorrect Coordinator version processing
     - Normal
   * - ZBS-2714
     - Fix payload of Discover Attributes Response and Extended Response commands for manufacturer specific attributes
     - Normal
   * - ZBS-2715
     - Implement DUT ZDD launcher for ZB Direct certification tests
     - Enhancement
   * - ZBS-2717
     - Fix APS fragmented transmission queue underflow when the new transmission buffer matches already processed buffer from the queue
     - Major
   * - ZBS-2720
     - Fix repeated attributes in ZCL Discovery Attributes Response command
     - Normal
   * - ZBS-2722
     - Fix error in GP subsystem which prevents certification tests from successful completion
     - Critical
   * - ZBS-2724
     - Fix missing the lowest attribute ID in Discovery Attributes Response in case of descending order of Attributes declaration
     - Normal
   * - ZBS-2726
     - Fix alarms queue exhaustion if the first End Device Timeout Request wasn't acknowledged after device rejoin
     - Major
   * - ZBS-2727
     - Add ZB_ZCL_ATTR_TYPE_S8 support to zb_zcl_get_analog_attribute_size
     - Enhancement
   * - ZBS-2730
     - Fix TC crash due to not found UNVERIFIED key on repeated Verify Key after Confirm Key
     - Major
   * - ZBS-2731
     - Fix rejoin procedure initiation caused by unsolicited Rejoin Response with network address equal to the current device address
     - Minor
   * - ZBS-2732
     - Add ZB_ZCL_ATTR_TYPE_UTC_TIME support to zb_zcl_get_analog_attribute_size
     - Enhancement
   * - ZBS-2733
     - Merging ZGP stabilization fixes after refactoring
     - Enhancement
   * - ZBS-2735
     - Fix malformed Default Response on binding error inside ZCL Poll Control handler
     - Normal
   * - ZBS-2736
     - Fix buffer corruption in zb_zcl_process_command_finish() Default Response for MAC-Split
     - Major
   * - ZBS-2739
     - Fixed potential synchronization issues when a device updates TCLK through APS Request Key during lifetime in a network
     - Normal
   * - ZBS-2740
     - Add earlier incoming frames drop in case of Repeated NWK frame counters
     - Enhancement
   * - ZBS-2745
     - Fix processing out of order APS ACK on relayed packet retransmission
     - Major
   * - ZBS-2746
     - Fix rejecting of APS group message transmission with payload smaller than the fragmentation size threshold when fragmentation is enabled
     - Normal
   * - ZBS-2747
     - Fix APS unicast payload truncation for payloads larger than 255 bytes
     - Major
   * - ZBS-2755
     - Fix incorrect relay usage for Security_Start_Key_Negotiation_rsp if a joiner has TCLK and performs ON-NWK DLK
     - Normal
   * - ZBS-2756
     - Fix processing ZCL packets with group addressing with fragmentation flag enabled
     - Major
   * - ZBS-2758
     - Fix sending Security_Start_Key_Negotiation_req via relay for ON-NWK DLK if device has already joined
     - Normal
   * - ZBS-2759
     - Fix incorrect processing of APSDE-DATA.confirm if APS header is not present
     - Major
   * - ZBS-2760
     - Fix removing TCLK on TC side if device stopped TC rejoin procedure after receiving Rejoin Response
     - Major
   * - ZBS-2764
     - Fix: ClusterRevision attribute should not be returned on manufacturer-specific Discover Attr Req
     - Minor
   * - ZBS-2773
     - Fix possible corruption of routing table entry while relaying route reply
     - Normal
   * - ZBS-2776
     - Fix incorrect TCLK updating via APS Request-Key if the device already has TCLK established with more secure mechanism
     - Major
   * - ZBS-2777
     - Fix incorrect placing Selected_Key_Neg_Method TLV parameters in TC Node Descriptor Response
     - Normal
   * - ZBS-2780
     - ZCL: Add missing float data types to ZCL Analog types
     - Enhancement
   * - ZBS-2782
     - Fix incorrect sending NWK status for ourselves if got Route Record command with disabled concentrator
     - Normal
   * - ZBS-2788
     - Fix ZGP pib attributes synchronization on macsplit build
     - Critical
   * - ZBS-2802
     - Fix corruption of addr_to_neighbor translation table when accessing cached APS keypair after removing a provisional key
     - Major
   * - ZBS-2803
     - NCP documentation currentization
     - Normal
   * - ZBS-2804
     - Fix a case when broadcast PAN ID could be generated
     - Normal
   * - ZBS-2810
     - Fix a race between zb_apsme_confirm_key_request() call and zb_debug_broadcast_aps_key() in NCP architecture debug build
     - Minor
   * - ZBS-2811
     - Fix corrupted ZCL header in zb_zcl_write_attr_resp_handler
     - Major
   * - ZBS-2820
     - NCP SDK packaging, including high level of Host in sources
     - Normal
