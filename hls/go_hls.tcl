# Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

source nvhls_exec.tcl

proc nvhls::usercmd_post_assembly {} {
    upvar TOP_NAME TOP_NAME
    directive set /$TOP_NAME/run/while -PIPELINE_INIT_INTERVAL 6
    directive set /$TOP_NAME/run/while -PIPELINE_STALL_MODE flush
    directive set /$TOP_NAME/run/while/mult_acc_1 -UNROLL  16
    directive set /$TOP_NAME/run -MEM_MAP_THRESHOLD 64
    # directive set /$TOP_NAME/run/while/mult_acc_2 -UNROLL  2
    # directive set /$TOP_NAME/run/data_i_1:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
    # directive set /$TOP_NAME/run/data_i_2:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
    # directive set /$TOP_NAME/run/data_w_1:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
    # directive set /$TOP_NAME/run/output_1:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
    # directive set /$TOP_NAME/run/output_2:rsc -MAP_TO_MODULE ccs_sample_mem.ccs_ram_sync_singleport
}   

nvhls::run
