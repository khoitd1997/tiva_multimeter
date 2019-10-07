#include "ac_bandpass_filter.hpp"

static const float32_t bandpassACfilterCoeffs[ACBandpassFilter::kTotalTap] = {-5.346939724e-05f,-0.0001009657863f,-4.873275611e-05f,-1.236608568e-05f,9.620528726e-05f,0.000164417972f,0.0001795283461f,8.787282422e-05f,-7.101448136e-05f,-0.0002337495243f,-0.000304450281f,-0.0002320263011f,-3.215386459e-05f,0.0002027522132f,0.000354303309f,0.0003402354487f,0.0001661378774f,-7.594446652e-05f,-0.0002589941723f,-0.0002963929146f,-0.0001920153009f,-3.331411426e-05f,7.082692173e-05f,6.661148655e-05f,-6.64287063e-06f,-4.647144669e-05f,3.165654925e-05f,0.0002170652151f,0.0003895291593f,0.0003884691687f,0.0001279900171f,-0.0003179973864f,-0.0007265271852f,-0.0008417624631f,-0.00053417444f,9.699318616e-05f,0.0007518810453f,0.001086266828f,0.0009094327688f,0.0003028704959f,-0.0004184499849f,-0.0008856452187f,-0.0008892297046f,-0.0004933296004f,2.105420572e-05f,0.0003468956857f,0.0003502800537f,0.0001445171074f,-1.089267334e-05f,8.25320385e-05f,0.0003951450635f,0.0006675142795f,0.0005877619842f,3.475939229e-05f,-0.0007841546903f,-0.001413734979f,-0.001419124193f,-0.0006785617443f,0.0004959406797f,0.001506966422f,0.001813343843f,0.001256607939f,0.0001608817111f,-0.0008652347606f,-0.001303728088f,-0.001037599402f,-0.0003924256598f,0.0001175008292f,0.000164971716f,-0.0001551451132f,-0.0004037660838f,-0.0001444080699f,0.0006803937722f,0.001622499898f,0.001972764963f,0.001243147417f,-0.0004309438227f,-0.002262455644f,-0.003223031992f,-0.002663360443f,-0.0007526756963f,0.001563061029f,0.003088280791f,0.00307693216f,0.001664543874f,-0.0002457799565f,-0.001573430956f,-0.001741228276f,-0.0009981106268f,-0.0001704852184f,-1.670121128e-05f,-0.00063870335f,-0.001377412002f,-0.001292614848f,8.757133037e-05f,0.002278781263f,0.004002034664f,0.003954247572f,0.001713558799f,-0.00183323957f,-0.004882068373f,-0.005736524239f,-0.003845266765f,-0.0001815759751f,0.003307906445f,0.004862473812f,0.003944112919f,0.00148515834f,-0.00081014639f,-0.001639250433f,-0.0009528256487f,0.0001030026469f,0.0001214993317f,-0.001405989402f,-0.003550005378f,-0.004470204469f,-0.002721420256f,0.001534033567f,0.006296044216f,0.00876083225f,0.00705076335f,0.001542254002f,-0.0051292032f,-0.009495325387f,-0.009284570813f,-0.004784523509f,0.001353573054f,0.005774370395f,0.006486568134f,0.003994058818f,0.0007456137682f,-0.0007182217669f,0.0003586276434f,0.002381423023f,0.002610092517f,-0.000642759027f,-0.006369887386f,-0.01106355526f,-0.01083107572f,-0.004159254953f,0.006540880539f,0.01583371498f,0.0182873942f,0.01184844878f,-0.0006088945665f,-0.01270299777f,-0.01821426488f,-0.01477291901f,-0.005203806795f,0.004406036809f,0.008726999164f,0.006659548264f,0.001878242474f,-4.807576624e-05f,0.003901320044f,0.01130726282f,0.01528384443f,0.009138143621f,-0.008110142313f,-0.02919417061f,-0.04141165689f,-0.03386437893f,-0.005094492342f,0.03379973024f,0.06331712008f,0.06553946435f,0.03472841904f,-0.01768818498f,-0.06730324775f,-0.08862765133f,-0.06865601987f,-0.01465987787f,0.04844586551f,0.09024169296f,0.09024169296f,0.04844586551f,-0.01465987787f,-0.06865601987f,-0.08862765133f,-0.06730324775f,-0.01768818498f,0.03472841904f,0.06553946435f,0.06331712008f,0.03379973024f,-0.005094492342f,-0.03386437893f,-0.04141165689f,-0.02919417061f,-0.008110142313f,0.009138143621f,0.01528384443f,0.01130726282f,0.003901320044f,-4.807576624e-05f,0.001878242474f,0.006659548264f,0.008726999164f,0.004406036809f,-0.005203806795f,-0.01477291901f,-0.01821426488f,-0.01270299777f,-0.0006088945665f,0.01184844878f,0.0182873942f,0.01583371498f,0.006540880539f,-0.004159254953f,-0.01083107572f,-0.01106355526f,-0.006369887386f,-0.000642759027f,0.002610092517f,0.002381423023f,0.0003586276434f,-0.0007182217669f,0.0007456137682f,0.003994058818f,0.006486568134f,0.005774370395f,0.001353573054f,-0.004784523509f,-0.009284570813f,-0.009495325387f,-0.0051292032f,0.001542254002f,0.00705076335f,0.00876083225f,0.006296044216f,0.001534033567f,-0.002721420256f,-0.004470204469f,-0.003550005378f,-0.001405989402f,0.0001214993317f,0.0001030026469f,-0.0009528256487f,-0.001639250433f,-0.00081014639f,0.00148515834f,0.003944112919f,0.004862473812f,0.003307906445f,-0.0001815759751f,-0.003845266765f,-0.005736524239f,-0.004882068373f,-0.00183323957f,0.001713558799f,0.003954247572f,0.004002034664f,0.002278781263f,8.757133037e-05f,-0.001292614848f,-0.001377412002f,-0.00063870335f,-1.670121128e-05f,-0.0001704852184f,-0.0009981106268f,-0.001741228276f,-0.001573430956f,-0.0002457799565f,0.001664543874f,0.00307693216f,0.003088280791f,0.001563061029f,-0.0007526756963f,-0.002663360443f,-0.003223031992f,-0.002262455644f,-0.0004309438227f,0.001243147417f,0.001972764963f,0.001622499898f,0.0006803937722f,-0.0001444080699f,-0.0004037660838f,-0.0001551451132f,0.000164971716f,0.0001175008292f,-0.0003924256598f,-0.001037599402f,-0.001303728088f,-0.0008652347606f,0.0001608817111f,0.001256607939f,0.001813343843f,0.001506966422f,0.0004959406797f,-0.0006785617443f,-0.001419124193f,-0.001413734979f,-0.0007841546903f,3.475939229e-05f,0.0005877619842f,0.0006675142795f,0.0003951450635f,8.25320385e-05f,-1.089267334e-05f,0.0001445171074f,0.0003502800537f,0.0003468956857f,2.105420572e-05f,-0.0004933296004f,-0.0008892297046f,-0.0008856452187f,-0.0004184499849f,0.0003028704959f,0.0009094327688f,0.001086266828f,0.0007518810453f,9.699318616e-05f,-0.00053417444f,-0.0008417624631f,-0.0007265271852f,-0.0003179973864f,0.0001279900171f,0.0003884691687f,0.0003895291593f,0.0002170652151f,3.165654925e-05f,-4.647144669e-05f,-6.64287063e-06f,6.661148655e-05f,7.082692173e-05f,-3.331411426e-05f,-0.0001920153009f,-0.0002963929146f,-0.0002589941723f,-7.594446652e-05f,0.0001661378774f,0.0003402354487f,0.000354303309f,0.0002027522132f,-3.215386459e-05f,-0.0002320263011f,-0.000304450281f,-0.0002337495243f,-7.101448136e-05f,8.787282422e-05f,0.0001795283461f,0.000164417972f,9.620528726e-05f,-1.236608568e-05f,-4.873275611e-05f,-0.0001009657863f,-5.346939724e-05f};

static float32_t bandpassACStates[ACBandpassFilter::kTotalTap + ACBandpassFilter::kBlockSize];

ACBandpassFilter::ACBandpassFilter() {
  arm_fir_init_f32(&_firInstance, kTotalTap, (float32_t *)&bandpassACfilterCoeffs[0], &bandpassACStates[0], kBlockSize);
}

void ACBandpassFilter::filter(const float32_t input[kBlockSize] , float32_t output[kBlockSize] ) {
  arm_fir_f32(&_firInstance, (float32_t *)&input[0], (float32_t *)&output[0], kBlockSize);
}