
trySerialize OK
---------------------------------------------------------------------
20160606-1
---------------------------------------------------------------------
OK1
---------------------------------------------------------------------
20160606
---------------------------------------------------------------------
SM9_H1_V2
SM9_H2_V2
已更改
剩余
ENC
DEC
trySerialize
---------------------------------------------------------------------
20160606
---------------------------------------------------------------------
task:
有以下可以确定仅需修改SM9_H2和对称加密和解密
SM9_H1(id||hid, N) 长度固定 ID+1
SM9_H2( M||w, N) 长度不固定 随M变化
SM9_MAC(K, C) 输入参数为密文，不需要拷贝
SM9_HV-----SM3------HASH  长度固定[2]ZZn12+[2]ZZn2+[2]ID
ENC
DEC
KDF     长度固定[3]ZZn12+[2]ZZn2+[2]ID

trySerialize
---------------------------------------------------------------------
20160604
---------------------------------------------------------------------


签名算法hid=0x01
签名私钥ds=t2*P1

密钥交换hid=0x02
加密私钥de=t2*P2

密钥封装hid=0x03
加密私钥de=t2*P2

密钥加密hid=0x03
加密私钥de=t2*P2


miracl.h 加载不同的 def文件

Big 去掉 def头文件

MIX_BUILD_FOR_64 MIX_BUILD_FOR_WINDOWS MIX_BUILD_FOR_LINUX MIX_BUILD_FOR_MAC MIX_BUILD_FOR_IOS

MR DEF

MR_PAIRING_BN
MR_NOASM

签名
双线性对的识别符 eid：0x04 
签名主私钥 ks：0130E7 8459D785 45CB54C5 87E02CF4 80CE0B66 340F319F 348A1D5B 1F2DC5F4
签名主公钥 Ppub-s = [ks]P2 = (xPpub-s , yPpub-s)：
坐标 xPpub-s：(9F64080B 3084F733 E48AFF4B 41B56501 1CE0711C 5E392CFB 0AB1B679 1B94C408,
29DBA116 152D1F78 6CE843ED 24A3B573 414D2177 386A92DD 8F14D656 96EA5E32)
坐标 yPpub-s：(69850938 ABEA0112 B57329F4 47E3A0CB AD3E2FDB 1A77F335 E89E1408 D0EF1C25,
41E00A53 DDA532DA 1A7CE027 B7A46F74 1006E85F 5CDFF073 0E75C05F B4E3216D)
签名私钥生成函数识别符 hid：0x01 
密钥交换
双线性对的识别符 eid：0x04
加密主密钥和用户加密密钥产生过程中的相关值：
加密主私钥 ke：02E65B 0762D042 F51F0D23 542B13ED 8CFA2E9A 0E720636 1E013A28 3905E31F
加密主公钥 Ppub-e = [ke]P1 = (xPpub-e , yPpub-e)：
坐标 xPpub-e：91745426 68E8F14A B273C094 5C3690C6 6E5DD096 78B86F73 4C435056 7ED06283
坐标 yPpub-e：54E598C6 BF749A3D ACC9FFFE DD9DB686 6C50457C FC7AA2A4 AD65C316 8FF74210
加密私钥生成函数识别符 hid：0x02 
密钥封装
双线性对的识别符 eid：0x04
加密主密钥和用户密钥产生过程中的相关值：
加密主私钥 ke：01EDEE 3778F441 F8DEA3D9 FA0ACC4E 07EE36C9 3F9A0861 8AF4AD85 CEDE1C22
加密主公钥 Ppub-e = [ke]P1= (xPpub-e , yPpub-e)：
坐标 xPpub-e：787ED7B8 A51F3AB8 4E0A6600 3F32DA5C 720B17EC A7137D39 ABC66E3C 80A892FF
坐标 yPpub-e：769DE617 91E5ADC4 B9FF85A3 1354900B 20287127 9A8C49DC 3F220F64 4C57A7B1
加密私钥生成函数识别符 hid：0x03
密钥加密
双线性对的识别符 eid：0x04
加密主密钥和用户加密密钥产生过程中的相关值：
加密主私钥 ke：01EDEE 3778F441 F8DEA3D9 FA0ACC4E 07EE36C9 3F9A0861 8AF4AD85 CEDE1C22
加密主公钥 Ppub-e = [ke]P1= (xPpub-e , yPpub-e)：
坐标 xPpub-e：787ED7B8 A51F3AB8 4E0A6600 3F32DA5C 720B17EC A7137D39 ABC66E3C 80A892FF
坐标 yPpub-e：769DE617 91E5ADC4 B9FF85A3 1354900B 20287127 9A8C49DC 3F220F64 4C57A7B1
加密私钥生成函数识别符 hid：0x03