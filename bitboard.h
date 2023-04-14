#pragma once

#include <string>
using UInt64 = unsigned long long;

// basic masks
constexpr UInt64 FILE_MASKS[8] = {
    0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
    0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080
};
constexpr UInt64 RANK_MASKS[8] = {
    0xFF, 0xFF00, 0xFF0000, 0xFF000000,
    0xFF00000000, 0xFF0000000000, 0xFF000000000000, 0xFF00000000000000
};
constexpr UInt64 FORWARD_DIAG_MASKS[15] = {
    0x102040810204080, 0x1020408102040, 0x10204081020, 0x102040810, 0x1020408, 0x10204, 0x102, 0x1, 0x8000000000000000, 0x4080000000000000, 0x2040800000000000, 0x1020408000000000, 0x810204080000000, 0x408102040800000, 0x204081020408000
};
constexpr UInt64 BACKWARD_DIAG_MASKS[15] = {
    0x8040201008040201, 0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040, 0x80, 0x100000000000000, 0x201000000000000, 0x402010000000000, 0x804020100000000, 0x1008040201000000, 0x2010080402010000, 0x4020100804020100
};

// dictionaries mapping index values to forward and backward diagonals
constexpr int FORWARD_DIAG_INDEX[64] = {
    7, 6, 5, 4, 3, 2, 1, 0, 6, 5, 4, 3, 2, 1, 0, 14, 5, 4, 3, 2, 1, 0, 14, 13, 4, 3, 2, 1, 0, 14, 13, 12, 3, 2, 1, 0, 14, 13, 12, 11, 2, 1, 0, 14, 13, 12, 11, 10, 1, 0, 14, 13, 12, 11, 10, 9, 0, 14, 13, 12, 11, 10, 9, 8
};
constexpr int BACKWARD_DIAG_INDEX[64] = {
    0, 1, 2, 3, 4, 5, 6, 7, 14, 0, 1, 2, 3, 4, 5, 6, 13, 14, 0, 1, 2, 3, 4, 5, 12, 13, 14, 0, 1, 2, 3, 4, 11, 12, 13, 14, 0, 1, 2, 3, 10, 11, 12, 13, 14, 0, 1, 2, 9, 10, 11, 12, 13, 14, 0, 1, 8, 9, 10, 11, 12, 13, 14, 0
};

// attack masks
constexpr UInt64 KNIGHT_ATTACKS[64] = {
    0x20400, 0x50800, 0xA1100, 0x142200, 0x284400, 0x508800, 0xA01000, 0x402000, 0x2040004, 0x5080008, 0xA110011, 0x14220022, 0x28440044, 0x50880088, 0xA0100010, 0x40200020, 0x204000402, 0x508000805, 0xA1100110A, 0x1422002214, 0x2844004428, 0x5088008850, 0xA0100010A0, 0x4020002040, 0x20400040200, 0x50800080500, 0xA1100110A00, 0x142200221400, 0x284400442800, 0x508800885000, 0xA0100010A000, 0x402000204000, 0x2040004020000, 0x5080008050000, 0xA1100110A0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xA0100010A00000, 0x40200020400000, 0x204000402000000, 0x508000805000000, 0xA1100110A000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000, 0x400040200000000, 0x800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000, 0x4020000000000, 0x8050000000000, 0x110A0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10A00000000000, 0x20400000000000
};
constexpr UInt64 KING_ATTACKS[64] = {
    0x302, 0x705, 0xE0A, 0x1C14, 0x3828, 0x7050, 0xE0A0, 0xC040, 0x30203, 0x70507, 0xE0A0E, 0x1C141C, 0x382838, 0x705070, 0xE0A0E0, 0xC040C0, 0x3020300, 0x7050700, 0xE0A0E00, 0x1C141C00, 0x38283800, 0x70507000, 0xE0A0E000, 0xC040C000, 0x302030000, 0x705070000, 0xE0A0E0000, 0x1C141C0000, 0x3828380000, 0x7050700000, 0xE0A0E00000, 0xC040C00000, 0x30203000000, 0x70507000000, 0xE0A0E000000, 0x1C141C000000, 0x382838000000, 0x705070000000, 0xE0A0E0000000, 0xC040C0000000, 0x3020300000000, 0x7050700000000, 0xE0A0E00000000, 0x1C141C00000000, 0x38283800000000, 0x70507000000000, 0xE0A0E000000000, 0xC040C000000000, 0x302030000000000, 0x705070000000000, 0xE0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000, 0x203000000000000, 0x507000000000000, 0xA0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000
};
constexpr UInt64 PAWN_ATTACKS[2][64] = {
    {0x200, 0x500, 0xA00, 0x1400, 0x2800, 0x5000, 0xA000, 0x4000, 0x20000, 0x50000, 0xA0000, 0x140000, 0x280000, 0x500000, 0xA00000, 0x400000, 0x2000000, 0x5000000, 0xA000000, 0x14000000, 0x28000000, 0x50000000, 0xA0000000, 0x40000000, 0x200000000, 0x500000000, 0xA00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xA000000000, 0x4000000000, 0x20000000000, 0x50000000000, 0xA0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xA00000000000, 0x400000000000, 0x2000000000000, 0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000, 0x200000000000000, 0x500000000000000, 0xA00000000000000, 0x1400000000000000, 0x2800000000000000, 0x5000000000000000, 0xA000000000000000, 0x4000000000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x5, 0xA, 0x14, 0x28, 0x50, 0xA0, 0x40, 0x200, 0x500, 0xA00, 0x1400, 0x2800, 0x5000, 0xA000, 0x4000, 0x20000, 0x50000, 0xA0000, 0x140000, 0x280000, 0x500000, 0xA00000, 0x400000, 0x2000000, 0x5000000, 0xA000000, 0x14000000, 0x28000000, 0x50000000, 0xA0000000, 0x40000000, 0x200000000, 0x500000000, 0xA00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xA000000000, 0x4000000000, 0x20000000000, 0x50000000000, 0xA0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xA00000000000, 0x400000000000, 0x2000000000000, 0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000}
};
constexpr UInt64 PAWN_PUSHES[2][64] = {
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1010000, 0x2020000, 0x4040000, 0x8080000, 0x10100000, 0x20200000, 0x40400000, 0x80800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000, 0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000, 0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000, 0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000, 0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000, 0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000, 0x10100000000, 0x20200000000, 0x40400000000, 0x80800000000, 0x101000000000, 0x202000000000, 0x404000000000, 0x808000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
};

// zobrist hashing
constexpr UInt64 ZOBRIST_PIECES[2][6][64] = {
    {
        {0xCB7356C32DB5176, 0x6D13F1305547960B, 0xBEABF27F2CEF644D, 0x524B067E41B34982, 0xB64E25D92314098B, 0x221E3001ED19C348, 0x58DD39C9AFD0F4D7, 0x4BCB3BE2A9A5E464, 0xC452500ABDEE5679, 0x95157CA10091EEC5, 0x8C091CBF6AF963B6, 0x14481C925A310978, 0xC1AC1531B9039B91, 0x6FA17D51DDFE017D, 0xD82ED0034EABC70, 0x6B95627B1D15A51C, 0xFB8E3DDA0E8758F7, 0x41585033B21AAC81, 0xB8FD90E206D10504, 0x625947A0038347A1, 0x81CA516A48C06219, 0x684E8CC6D6104822, 0xA16C96C2094C188B, 0x5799FDD72496DEDD, 0x7DB68B16149B37C9, 0xF1C3136249E3D527, 0xDA6020E49DDFB176, 0x5D1C0DABCBAC2C7, 0x312CFF93038FAD93, 0xD74ECC05DE439637, 0xE8248FEEB5B1A593, 0x47003FE6CB1319E0, 0x151C315640FCC06E, 0x2818D3962D19A167, 0xAA7C88B0F15935DE, 0xB701920C3BAA50D3, 0x9212A271D08E668D, 0x4397AC93B51369C7, 0x9E4AB4FEC5FD012F, 0xD656FFC481ED8173, 0x6733B075B7A7B5EC, 0x98A128375CA0E216, 0x8EF74B9C1B4007F8, 0xC703CD2246C3EF57, 0xC95E1738933BF9A5, 0x10EF8ABDC729C53D, 0x39F853474F4D977A, 0xCB0BDA0D7E38C540, 0x9F07FB566459A4AA, 0xDB15DC360A284677, 0x481EDCC08B8B4538, 0x6AA19E7D443569AA, 0x9F9B7CF02FBE4E6E, 0x2D723DD284BAF1BB, 0xACD9F48786C8A902, 0x6C9C16B9FDC7314A, 0x7D20D68D2C497A14, 0xFB6B2A09194C2D95, 0xC7FD2D1160030462, 0x8541EF7744EE9D2C, 0xFF2C0C2903521505, 0xE0336D3454219DD7, 0xEBB0B7B043A0965B, 0x63D7D9ED0C4A4432},
        {0x9DA5B9FB6E92ADBF, 0xA8A59531929EDD22, 0xC8BF2E82DE037DED, 0x4A9C7486E7D18BA4, 0x7F674297EB678DE3, 0x4F489F62E59EF272, 0x1E4891E7135769C0, 0xD250CA7766341641, 0x2940DEF3F732BE9, 0xDA1CC18587B0E9D3, 0x668A1A9E3D7FE307, 0x911C9DA0920A82A2, 0x655666A68AC7F20B, 0xC50EE54526686438, 0x2BE68E165E01FDC7, 0x15801F4D86EDE98, 0x1E0C1C0EEAE29931, 0x1B81528A0B0C10D6, 0xBC30D4E7B5AE55A3, 0xFCAA30BC2AAFB788, 0xC874E3DB9B413E2B, 0xA9759D922949E9C8, 0xC8DF06DF496D9517, 0x73F442F9591772C8, 0x88B0F81EC53FAA7F, 0xB14B0A52597B7597, 0x23FE8E9EC043D47D, 0x754619966B4E5D3A, 0xC65A6408FD7997B7, 0x864E80EEE22F9BCD, 0x1B99CDA906373B9B, 0x8B7D784826D5A864, 0x976862BD77481816, 0x400B475A76E8419A, 0x2EFF7E0F6617B8D, 0xB830F0BA50372FA5, 0x7A5120036BE763D3, 0xC4CDFCACB3DDF8A7, 0xDEB4D434FF37555E, 0x195646D3DEADEDC2, 0x147D5B61E8062F0A, 0x12FA9A653CF821F0, 0x765E60CC06CA9A4, 0x12CADCB812BB5758, 0x76238390808EDB65, 0x416C2383D550B7A2, 0xA08B43B37DCB2C78, 0x63200930E4BB7594, 0x8E7A87D13D18DF8F, 0x9D845A06574C8F10, 0x939876550B433F4C, 0xD502DDDB21BBD70B, 0xA5AE3B754AC59DD1, 0x1BAC09A9D70E1AD1, 0x95F2D7C00F98CB26, 0x7F8530F6055D5FCF, 0xAC036C64C5D4D6F8, 0xDFF59975AED221B6, 0xAE8761EE4AAC35B7, 0xAAFBCAA62DE06E44, 0xB839E23E2E3270E4, 0xCFE8C14A5FF1A551, 0x91A5661649BA628F, 0xCD3DCBBCC7778498},
        {0xDC0C4A364E8476EF, 0xB0D09F0FE5FE988A, 0xEC6DB8617AC5C8D0, 0x28ADA7817C29398C, 0x23557BD06C6221B4, 0x767265842876CE1E, 0x9DD6CBF8551F7DDE, 0xBBA8E70A6E61E8CC, 0x48B72112F731338F, 0x3DD2FF277BB78F14, 0xF79893AB3FDB1DA8, 0x6266E25AEDC5ABC8, 0xF1C0AE23EBAB5BB7, 0x725B014E45CE1D44, 0xCC9123595C39CA8F, 0x7BB866D31616540D, 0xAFD307D0CAEFE0FD, 0x1C3C720198963560, 0x5925C8996C3637B4, 0x7EAA21A1C56AAF58, 0x32C015F52D65E601, 0xB7DECB2D0C526720, 0x77E7594E5E7F87D5, 0xEA0FF9115EEC94AF, 0x49A8A8149A427DFE, 0x9CAE2AA91F848E3, 0x7B71D502F28627F6, 0x5BBDC9120FF2C588, 0xD33D99D9D785D746, 0x6DC393A94A6230AC, 0x9E0F0CC38C6099FB, 0x185131FD8FF5ED35, 0x18A95F5A437D0F55, 0x76DCEF0FDA6B541C, 0xC4DFC28AEF347927, 0x42F97987A09164F8, 0xC3CAC210944425F, 0x58F5604394FE453, 0xE05095CC18430E62, 0xFEB8BDC1A73B2758, 0xE7D389F3CDD09081, 0xC1D83A4F1D9980A8, 0x1B0D4A6C7BB7C38F, 0xED51C46DCE1B7F75, 0x9EE775CE6F3C297E, 0x84E92A87D4541D29, 0x1CAD188291FDBBFD, 0x5C5D13ABCC41320F, 0x16B53A5B0E8EC46B, 0xCB1CDF3D26524B0, 0x5EFC0130B714098C, 0x9CDC1B414BBC9AE5, 0xEDA4C9E436C6AEB2, 0xDB63F02604618E4C, 0xF968806D855BF54D, 0x6420DF32820A7A75, 0xCD4EFE3FC2A14DC1, 0x7074CDFFAE421A2A, 0x8E3505AF2A01543D, 0xF3FB9F5F04DBB662, 0x49906AC975002F75, 0xCC9202BD10B1B987, 0x25E4AD992B51F631, 0x67BC8BA419D99C91},
        {0x58AAAEFCDED369F1, 0x716E6C048403FE9, 0x4E447DD3345298B4, 0xAAAB77B75873F4E5, 0x42AEE375E46785E7, 0xC8D80602F560519C, 0x72E6383900A1E4FF, 0x7C9F2335BAA359E9, 0x2BB98948E752A973, 0xA727A7D61070100C, 0x69AB53CDD1B1023F, 0xBD5F9570E2EA3749, 0x12E9D4DEC90A8231, 0xCE878C5202493FF4, 0x3CAF7C0615A3808, 0xD113DDA9B779F798, 0xD4738376BFC9C73F, 0x7CFC1140591835AB, 0x4862E31D466F93B8, 0x6F8ADAD73E393394, 0x62C2F014039865D9, 0xB0CE3527127A302E, 0x3AD71A1C3F7E2EB0, 0xD9A887C218E18D3A, 0x5D67C511F3F4B2C, 0x748FD341C818E51E, 0x1645840E554CA560, 0xF60EF66E05E32DD2, 0x2B9D1BD1D5C5035F, 0x608B6F3990DAB5DD, 0xE4349E7588CEBC27, 0x1710DD9E5A8F6ADE, 0x4D16D631178F8F83, 0x2C59C00CDBA6095, 0xD2A3F15751C97A2, 0x862EF40FDBADE15D, 0xE164C6EDAA5533D2, 0xE05F28C334C17A49, 0xE1D16AE58F61AE3A, 0xAD5BF41D26177CDF, 0xE30F6FB06CE3B06, 0x71687BFF8E003DEF, 0x27EF6EB59D052A9F, 0xBD86F840D730395E, 0x26EEF30ECA212EE8, 0x25504561B43978FD, 0x87BD35FAACE356C4, 0xE2CF969709B3C8E1, 0x66E4CBB55AAD70D3, 0x129426AB89A4553E, 0x46631DCAF0382B80, 0x9B2A372074D3512B, 0xD355264AF246D6AA, 0x7C5B62AFA24D0602, 0x71066D9B2954182C, 0xF3EA78AC2D88000, 0xCB1206308685F494, 0xA0AB68DB474869B3, 0x4BB067D027CD742D, 0x6984983D236E7C40, 0x436027E00A682691, 0x17C620E3DA1C11E7, 0x501DD858B0ABC038, 0x4948577D2C4D5E65},
        {0xABB33DE930C94EB9, 0x3213FE6C3FC32FFE, 0x3E42F134E42A2E7A, 0x5DB88B7942B60EB, 0x5497B73478C40361, 0x3C3E6D8A7C9B0D20, 0xFA53364D49A5BB07, 0xF11312D321BCB0E1, 0xEB4E4B0D7E686926, 0x5A08ACA2CAF14D0C, 0xEE74CE3852F5689D, 0xD0E1053780625509, 0x9A06003314708323, 0x737B500F95757872, 0x404E1D19EC985D6E, 0x3FBAB53F92BA44B1, 0x2EA7002C9B52272A, 0x66C61253479B0657, 0x5055EA02B86B1314, 0x6E401B78E0C58018, 0xCC5F5D75C4A5759C, 0xE9CFA30BF103C35B, 0xAB5E23B3709A8EB6, 0xD38D0B48BAC44D1F, 0x7E2905699868702A, 0x6D66997A38BB835D, 0x4352F96EB2728A43, 0x407F32E6CB1DC5B3, 0x2CEBA984091A9EED, 0xD0A764643AF19763, 0xBE0D5C4AD9313266, 0xDB3D6656FAE3283E, 0xC71FFC5F76036C4F, 0x861D502DB8604C3, 0xB18C18F71E5E4A60, 0x921E414D80B635DF, 0x1DEDB3167DEE7A20, 0xC067255A3BC8C69A, 0xB6B29FC721C5D255, 0x7620E850D021323B, 0x210FCABF11872870, 0x9EE3037CEDCEC2A6, 0xE237A09C1E2F59E0, 0x649D8B16E6A282F2, 0xD9AE01CA6C8A34F5, 0x467CCC984B050831, 0xA7CE97ADBEFECF42, 0xECA8C51BF2114095, 0xB45A93E2DEC57C4D, 0xF402C41F39AC612, 0xE2D3ECEFE246DCAA, 0xFDC675962AB6F6F6, 0x69FD62B867DB83C0, 0x15EC9D67FFB7839E, 0xC5ACE386820EAEC7, 0x29DD35F7E00198C8, 0x652DCAF24726136A, 0xB9204A9B29CD56B8, 0x8BEF5ADADB59A613, 0x5F38F6EFD94C50F, 0xCF166F5BFE5C5C52, 0xB635BDD4E573FC39, 0x6AD5D4C562239F39, 0x13D36D4D71BD5BAA},
        {0x66849D81C62D4086, 0x4CF1B96A302BA053, 0x6068B0492E047652, 0x1A11CEFEE8A35FD6, 0x29FEBF98D5990D51, 0x1547E75B484B0F82, 0x8AD962ABA4DF2C4A, 0x2ED4CBAD48BCD1D, 0x944BCD75D88CF82C, 0x8ABBEF598567AE7F, 0x1EBF379CF088DF51, 0x41FEE2006B15FD90, 0x377CA011F0AEDFD9, 0x5767FD2D9409B688, 0xE431A60E0BF123A3, 0xC40BFAAE94CF7127, 0xE761759136FEA6E9, 0xBF5D79E95F2890DE, 0xC5932D798EFDCBAB, 0x2F64FC8381D76CE7, 0x403F2E1762E2C65E, 0xB7FE61232FA97978, 0xB760E911B4748918, 0xCF80E3B62FA2DCEE, 0xCC98EF3CE020A72, 0xBF3CD8610D4F6738, 0x7C08DFA91F99043C, 0x4373528E811C0852, 0x9BD7AD99D7E5A8E, 0xAA7ED676BA852CA1, 0x3364B8446137E304, 0x832B7F4DC9C1B368, 0xB8DC8CD1C81728FE, 0x358AED9B3B1A05A2, 0x1055BB2956A19557, 0x2BBA6795B0835746, 0x879E49637EBB988E, 0x3A657B9C70129914, 0xC79061B164606AA, 0xE005DEEDBC312042, 0xE460B647ABE23519, 0xA900DFEF6E495D7B, 0x440743F5ACE11952, 0xA903898E34F27396, 0x6D779CDFEEFB8FBE, 0xC94DB662940AD014, 0xD7C70002B02C71E0, 0x82DE665C86B8B0F4, 0x5A94D8B887F5BDA5, 0x2BE63953BCFEC511, 0x17582E59ED122360, 0xF44C03CAA89E6A9C, 0x6C856A74604B5F05, 0x3CECA653783ECEE3, 0xA362270CFE4BD7EB, 0x5293BA5D697F11C1, 0x6086FFF46E52EBA8, 0x8B3297B2A04CD2D2, 0xC58E7AE4F659BDEB, 0xF8BB2EA0EAE99AE4, 0xC4F4B4F06EFC0808, 0xF4423E0D4F707A6E, 0xC82C519176624BD3, 0x732CA186E2D6B4BF}
    },
    {
        {0xA062B16E20E8B193, 0x5959A93944868181, 0x12B3AEE08A051FB, 0x83C9EBAF5F0F06EE, 0x509FB4C63777772B, 0xDFD4477BB45AA16E, 0x120E3F19EB9C950, 0x257CF09D0F50C23C, 0xF567FACDB79245AC, 0xD3712510058DF076, 0xD08DF4FC3179F8CD, 0xE0F4870DDC6DD5ED, 0x1B7926D29530785E, 0x1FEC046C62348E0, 0xB07E99B8DEC7999E, 0x514C3A2455F4C081, 0x2C0954A7542C47D5, 0x48326D913AC8958A, 0xABD1F55961356EF1, 0xD95BD11BFAA696E0, 0x48B0A61BD086883, 0xA3FAB7EB805368F4, 0x66EA42C4B7CC03DF, 0xDE32E222FEC94D91, 0x7F0607D468BA7CA0, 0xCEBC436C63B9D3B, 0x411F20E263D18D35, 0xD080E8DBA112F7EC, 0x3F6B9804AFE3C09C, 0x345B7BF0D47EEED5, 0x27E5F9C08823311A, 0x6F71B034E1E1E89C, 0xB6E29F2E18143D83, 0xAFDA6A7668ACDCE8, 0x410AD342035620F5, 0xD4C3EB025D652D42, 0x3E530D1FE0350C6B, 0xB3D32247E290DA60, 0x917CBBF834417F5D, 0x46D48B7C0A809711, 0xFEECB58B2E8CB9DB, 0x326C6CA0041E7F32, 0x59CFFA71220F5EE3, 0x3786B9C21138692F, 0x6E0F20FEFCF82EB9, 0x5417A74B5F234907, 0xD44289C58CC31559, 0xA5E27B7826DB6A0A, 0x7B22A39F6E3AD5CA, 0x333580DF64EFF64E, 0x91C0A5C8954390A1, 0xE07B26F6DC78BB21, 0x8388F7E757D7A17E, 0x7ECD6FF9E53F8A25, 0x76B75B79BD89A290, 0xB80012A04A571473, 0x2153B96E6FF2AFA3, 0x3928CF4455AB6BDD, 0x6B79DE8AC8950DDD, 0xB701F5CC32F29617, 0xE1D5C1565FEDEB5F, 0xBDE724353DC5F5EC, 0x6CE9CDBAB8EA8299, 0x8F162457C42BA421},
        {0x51B9F7E95FD04655, 0x20B7B0C95E423426, 0xEA625F6623089A32, 0x5A50143D967E9EA, 0x5ABB6796ECEF29BD, 0xB540E9FF0541FDFD, 0xA73B10FF72C149F8, 0x890152EC41895938, 0x46CD3BB8C6458059, 0x4810B1D52BF46F3E, 0x419E2ACAAC3E6E42, 0xF040AB39187425C9, 0x8F141138C627C2BF, 0x28609DE7A2CB1372, 0x418D1B760ECA382E, 0xC162FB497BAD2521, 0x86F7863CAA305266, 0xA97705EBF40FCC12, 0x6609DFB1DE7446F1, 0xEA30F1BA63C4D694, 0x75CFFB4172DA5159, 0x872A27B749190C7D, 0xF6A2891D21F5582A, 0xD4AC15CF8F70F8D0, 0x25BCAE88002A90A3, 0x6E1740B40CA293CB, 0x7CFFE63247D1295F, 0xF7AEF84180BF5811, 0x8569DA25407F8055, 0xD6054BF49DF1C0F0, 0x7EDC09045A19EB59, 0x87211E052CCE08A8, 0xE0DFFFD7111C7093, 0x18A897AADF2A2C17, 0x8DAF3D087F185A3B, 0xD3F7234C9BF6D333, 0x50EB904064BAB198, 0xFA21684FCE8CA491, 0x6FAA0C5605773FC0, 0x251616AAB1D436FD, 0xED891D139AD3528A, 0x3DC7507964BF5C0A, 0xF6465CD69F644720, 0xED8EEF25B75E0798, 0xC74DA0B3E235011A, 0xA5F690E5DDB98EF4, 0x4E9B23F6DC5A9BF4, 0x2C3E8DABD4203555, 0x5D55CA04D296317F, 0x2608092CE5BFC9CF, 0xCD6A37EAF06DBB84, 0x9FB2B060378D7610, 0x45376E7752B7B4, 0xC707CFCD67E240DC, 0x3F35FD08F584D844, 0xFE9E42419DF8857, 0x574CB66AB7A0699, 0x6C1FEEF12A2A454A, 0x770C3F7AF7910CAF, 0xBAA87D85DAC5022D, 0x9671F47BEE39A851, 0x1E75C3023C2AB88D, 0x1C1FA8355D2E4D8E, 0xDB6D901A59B4690A},
        {0x73D446D133CC5477, 0x4E57928FCFE707AD, 0x7386E3FFBCAC7A3B, 0x14A3E8C7F68CA44A, 0x37E2FA811BA0F3E1, 0x5BD6B47BA95075E3, 0xBCB3C2AEFBFDBE62, 0x4F97F4DC079B2E1B, 0xA06797018801A5AC, 0x622ADD546B7BFB82, 0xBD103BA527496209, 0x25FF5886A047CD8F, 0xEC8701E1B490DA19, 0x4C8C6C50AA8BA47D, 0xEFDF85DD1A853A03, 0xA4BCFE00773A8FD2, 0x28FB79187730DB75, 0x6698DACB2457A211, 0x4F1805E550F9C89A, 0x9C1F8A7B8CCC7A95, 0xCA0B646C49F17A0C, 0xC34E67BBD21D279, 0x4B713EA0E50CACB7, 0xCF65A041A47C7230, 0xB37016D32C0553F2, 0x471F07D645889E19, 0x394E9A8DDA9A46A9, 0x2761A8965FB6F77B, 0xABD2EABAA6A26D85, 0xCBC45A22EB4D0B85, 0x265FCF95E2F8F384, 0x90107C6DFE290717, 0x56E4F8C97244D7D, 0xDD7130593E1FC80E, 0xD7FD0FBDCF18BE3B, 0x632C00AC7BC37305, 0x9B1A348952B9A221, 0xAC768561EC4CA10A, 0x32CBBEAC9ED6BAE0, 0xB6881BDCF6CDC739, 0x39269A18F945CC29, 0xD2BA82CE4F0B93C4, 0x65BC8DCBD9B45BD2, 0x4E85AA06D7DCEB40, 0xAA4906EA1E6D0625, 0xD24A9019705A6E6F, 0xA88C37DED60D8E51, 0x173E95757EA68494, 0x5DC7A54A9CF0AE23, 0xDEEBBB2131B8AD57, 0x81B1D1CBF890E7CE, 0xEF0314FD2FCF47E0, 0x6561F319A3B3F379, 0xBBB035F4026FE64A, 0x357BC320155F8A0D, 0x3E47A8A65AE041BD, 0x6A5D7B8A5E3576A3, 0xE0E587624F2B0157, 0xF5BE75A9AF39F6E0, 0x7ED7379134A383AF, 0x3D0C793C770EB679, 0x9BB5603DD0B47D5, 0x837BD582C4373A53, 0xA502DC73AF662AD4},
        {0xA43018E1A3F7FAB0, 0xE264C97973D0E431, 0x5688863EC03B347F, 0xD349E1D37B1600E3, 0xAC8F00035B25FAE7, 0x2404AC5ECAB2EEDA, 0x21818866B6205B4E, 0x53DBDE342263FFE1, 0x3BBD01F85A924235, 0xB54E3B1C9311A87E, 0x2243806393ACBCF5, 0x3F7DF55CBCAD495F, 0xC8EB5E0F20ABF987, 0xD2282D54987B67DE, 0xB19BD0394C9CF9D5, 0xED3C826F615A09F, 0xCACD844D07906E1B, 0x8CFB4C0E07B1348A, 0x20A4C3B1FFE469, 0xD7CC348AF0778BC2, 0x9A4AA19130ED447E, 0x919CF7D944EE0BA, 0x68BD0FDAAFFD0C28, 0x3E356029229A9506, 0x7F207B8C5C9E09EE, 0x813DC25BB170CF42, 0xA1804B3DBC42F9ED, 0xE972065361AB0271, 0x1C4B2C080657FB8C, 0xCE26DA322C1F788F, 0x28AFCDB2BB6E9C, 0x7683993DD2217545, 0xA40906686E6FCA47, 0x20D3C014B7A68A2E, 0x458A388F1ACB1049, 0x241E901192B57FB7, 0xF07163DD6512A9C6, 0xAD1DF43A2E5A6103, 0x29790A264B79D0C0, 0x9B7C62C8EF2D529C, 0xC3D4A9525BF133C3, 0x3E5E6EB16AF1AB42, 0xF3D7661E9B45EB39, 0x99B427FF0CE27CD, 0xE7BDA948487DFF1B, 0x81A09211DED80E0F, 0x3B3808B140F6A5DE, 0x95A596292A3703, 0xE66D7287ACCF21D4, 0x37EDE7AAB9DD8C2F, 0xD488CC50E85CA0AC, 0x23F55B032048E5D2, 0xB8E394C5AD220767, 0xEDBAEBBD61EDDA74, 0x62A8003C135ED5CE, 0x7B6A0B2E26B7F130, 0x390CF0A53B7B2137, 0x969A54697B4F8CA1, 0xCA6A1666F7B7EB83, 0x9D045097541E2FF5, 0xADAA1A83316C0200, 0x364B3E9E2BEE4274, 0x92765537A1A441C4, 0xAB24AF042E45C988},
        {0x3FB5ABC82F6A3977, 0xE5CF55F9F8738406, 0xEC82D1566660012A, 0xE5002FFF0CECC6C2, 0x7EA13D7476DDA961, 0x5C3FE3DE10F47388, 0x2480001EA06D023F, 0x1E008164DDB1EA91, 0x714B4102DBED053C, 0xB97698E61971ACC, 0x9A0E2E80217ABDD0, 0x9D69F087484A3D65, 0x8D2387AAB4F2C4A1, 0x74D23CE23D7BF9FB, 0x171DA75DF37360A9, 0x7EDFC84AC97A7A36, 0x252342E2279C0EF5, 0x4563BF2EC9ED61E5, 0x1E4C88A42DDAB9C6, 0xEF44406C4568FCE8, 0x70AAB75B3F4AC827, 0x2E90A79F868E49BE, 0xC370F57C633D24B, 0x48D891883014DC67, 0x1A009DF153EE7CF1, 0xEBFC789EBF5C21FC, 0xED6DB40D00C5609C, 0xF4B0EA34910E03AA, 0x8C105890646922A3, 0xE5760837EC77C6E0, 0x2FAED811B53383E8, 0xBB62D49135C8815A, 0x101C3D9348C12F48, 0x15553EFAC58A96C2, 0x8A2052D959D7193A, 0xEAC968EA131ACB58, 0xA6B60665723E9686, 0xAF9FF54D2989A70A, 0x130B644170DCB992, 0x38F42CB0A54EEF58, 0x8F9D751C66451C3B, 0x212F6713618DB1CD, 0x6939118CA95E6B52, 0xB558661529075181, 0x2F51279817EE034A, 0x5734B051720F1ADC, 0x20FEFA2BA6B816F7, 0xDA5A6788C6994E61, 0x150D800A8F3AF49B, 0xF21D15737DACF909, 0x35163800D48CD3BE, 0xC20493440081DF8C, 0x5B2EC9126FF406FD, 0xA41AF711054E90D4, 0xC97E249DC8E421E, 0x1647C8A19C6ED1E9, 0x747FB555BBEA5C11, 0x9714B6A4020D22DB, 0xFAA65D9A2FB21C27, 0xF92C61027EE05E85, 0xFC48652A57E38360, 0xE84D10D907F38449, 0x294FACA85F703DC2, 0x81C565566521AD16},
        {0xECBF33E5D85DDA5B, 0xFF759EB5CDFA36E6, 0x74975288B9BF1BB8, 0x422A32ECB48F0911, 0xC59203C1563077C7, 0x6775F3F094136A79, 0x31C389CF7CFC91CD, 0xCC8757766531648, 0xB13C0E6F8ECA9B41, 0xE8DAED295661FEF8, 0x48F106F155BA188, 0xC7333AF611650A2F, 0x959C49BE73EBC02F, 0xFFBB7209D0046219, 0x7E2E19DF02959C28, 0xA7737F9C9E8CB9CC, 0x86846BA0D2C9DB9, 0x196CE52DFABB614, 0xEA0C9F442FFC743D, 0x6FB9E51094B41B9A, 0x708DA358D203DB2C, 0x16882C7E5D210748, 0x9581575081FC7C0, 0xDBCC5C1358DA90EC, 0x5B33320AAD19BEFB, 0xD90F5564512CCA19, 0xDFBC74C732DD6C66, 0x6171F62B5D7269A3, 0x964AC3E4C0DC9E53, 0x9FEA94C23734598B, 0x1BD09176E9E2E20D, 0x175F6B91A2FF2605, 0x8C753F8E59C8A57, 0x541344149AE48EED, 0x3C693DCEADDE7F58, 0xDFC6BF135DF207DF, 0xAE741B924B0DD4D, 0x3FED802CAD04DBF0, 0x851A2A05C1802D66, 0x21053313B5D2263F, 0x1510DC729E5D4430, 0x6BE7957E2F5F04DD, 0xF35FAD0D253B55B, 0x471B786A18F9BA18, 0xB3BD1D3CAF3430CF, 0x5B1C8531B50A7B24, 0x7CB39DAAD4065329, 0x4EE6A56FFDC38BDD, 0xFC1BDECAC0262B01, 0x13C7410FDF243BA9, 0x774A2E7F50F4037, 0x6A81BF3C1A235629, 0xAF0FC641940422C1, 0xB1EF547E7428CF33, 0x32D9FFF8532A3215, 0xF647742E3EECCFA2, 0xA63EBF55504E3EDC, 0x4BC6D8DF79FA1608, 0x30C7AAA2D723E15C, 0xB7E72BEECC10EBB, 0xC91CBD1BE82FAAFA, 0x224E8922AB3A2A7D, 0x76194C91A8EC4D8C, 0x7D78F82F775B2EBB}
    }
};
constexpr UInt64 ZOBRIST_CASTLE[16] = {
    0xBEC5C4C13E0473F5, 0x79CDA3074724EA23, 0x634B1347228C32F1, 0xD84B9FB8B9E1F804, 0x3FAF1B96F2CD4B8C, 0x636DD4EAFB1DA972, 0x23424C97D489F3D0, 0xB9C24A73830FEB46, 0xA98733628FE1264B, 0xA11197FA5C32418E, 0x9BF19BA083576AAC, 0x96D3471124E82F5B, 0x83D219691D1DF1C9, 0x45BA559DA73BB565, 0x7CE1D9B2A42A4EB4, 0x5B68972AD11C8248
};
constexpr UInt64 ZOBRIST_EN_PASSANT[8] = {
    0xF8068A52B4A36DF6, 0xF8F16439CDF1F7F4, 0x2DC50872328F0EAD, 0xED75B70E154BF2, 0x2EC08C61FE1C7695, 0xB9A1D61973509790, 0xE0F48D769E265041, 0x41058CAEC1FDC646
};
constexpr UInt64 ZOBRIST_SIDE = 0x65FF81B675E51ADE;

// basic masks
constexpr UInt64 ISOLATE_BISHOP_MASKS[64] = {
    0x40201008040200, 0x402010080400, 0x4020100A00, 0x40221400, 0x2442800, 0x204085000, 0x20408102000, 0x2040810204000, 0x20100804020000, 0x40201008040200, 0x4020100A0400, 0x4022140800, 0x244281000, 0x20408502000, 0x2040810204000, 0x4081020400000, 0x10080402000200, 0x20100804020400, 0x4020100A040A00, 0x402214081400, 0x24428102800, 0x2040850205000, 0x4081020402000, 0x8102040004000, 0x8040200020400, 0x10080402040800, 0x20100A040A1000, 0x40221408142200, 0x2442810284400, 0x4085020500800, 0x8102040201000, 0x10204000402000, 0x4020002040800, 0x8040204081000, 0x100A040A102000, 0x22140814224000, 0x44281028440200, 0x8502050080400, 0x10204020100800, 0x20400040201000, 0x2000204081000, 0x4020408102000, 0xA040A10204000, 0x14081422400000, 0x28102844020000, 0x50205008040200, 0x20402010080400, 0x40004020100800, 0x20408102000, 0x2040810204000, 0x40A1020400000, 0x8142240000000, 0x10284402000000, 0x20500804020000, 0x40201008040200, 0x402010080400, 0x2040810204000, 0x4081020400000, 0xA102040000000, 0x14224000000000, 0x28440200000000, 0x50080402000000, 0x20100804020000, 0x40201008040200    
};
constexpr UInt64 ISOLATE_ROOK_MASKS[64] = {
    0x101010101017F, 0x202020202027E, 0x404040404047E, 0x808080808087E, 0x1010101010107E, 0x2020202020207E, 0x4040404040407E, 0x808080808080FE, 0x1010101017F00, 0x2020202027E00, 0x4040404047E00, 0x8080808087E00, 0x10101010107E00, 0x20202020207E00, 0x40404040407E00, 0x8080808080FE00, 0x10101017F0100, 0x20202027E0200, 0x40404047E0400, 0x80808087E0800, 0x101010107E1000, 0x202020207E2000, 0x404040407E4000, 0x80808080FE8000, 0x101017F010100, 0x202027E020200, 0x404047E040400, 0x808087E080800, 0x1010107E101000, 0x2020207E202000, 0x4040407E404000, 0x808080FE808000, 0x1017F01010100, 0x2027E02020200, 0x4047E04040400, 0x8087E08080800, 0x10107E10101000, 0x20207E20202000, 0x40407E40404000, 0x8080FE80808000, 0x17F0101010100, 0x27E0202020200, 0x47E0404040400, 0x87E0808080800, 0x107E1010101000, 0x207E2020202000, 0x407E4040404000, 0x80FE8080808000, 0x7F010101010100, 0x7E020202020200, 0x7E040404040400, 0x7E080808080800, 0x7E101010101000, 0x7E202020202000, 0x7E404040404000, 0xFE808080808000, 0x7F01010101010100, 0x7E02020202020200, 0x7E04040404040400, 0x7E08080808080800, 0x7E10101010101000, 0x7E20202020202000, 0x7E40404040404000, 0xFE80808080808000
};

// magic bitboards
constexpr int ROOK_INDEX_LENGTH = 14;
constexpr int BISHOP_INDEX_LENGTH = 11;
constexpr UInt64 ROOK_MAGIC_NUMBERS[64] = {
    0x76E49D1E9B449768, 0x604C92B5723A7C1B, 0x725E76D651B3C241, 0x69C6C164733CCA50, 0xD28383DB970B36BA, 0xB1CF96B0DF1B9F60, 0x6B8684BA5AAC29A8, 0xFBD66723B5020C59, 0x359EA95A7E3EC86A, 0x24BF63EA73F7584F, 0x3FF136311CDD72F8, 0xE7E3252E00D7C2F9, 0xFA35A582E40756E1, 0x1C4974E1CD80534C, 0xF9072196F26D6BED, 0x2242602CC0528DFE, 0xA75B4D8C79358B34, 0x4034117711E92D5B, 0xF7A04593026D7163, 0x993ADA8415ACCBDD, 0x13FF597F2BE410E1, 0x2F53942FADFEABEA, 0x66DF02B418DA9F10, 0x3E6B62A4C8FEF2EF, 0xBC3B8EFE2907C20D, 0x2D218D8968122DF3, 0xF4B1D61DABA239D1, 0x2A196076BE44B3C6, 0xF951C6568FBBCE29, 0x63978C46C8254E3F, 0xA9D480EE1AB68706, 0xA13999E458E0E340, 0x5A38E027DC6BE542, 0x826D2B1A462D8E61, 0xCC27DCBA993F81BC, 0x4C0F910E734D73C7, 0xF4F9278C2BCAA0C0, 0x9E3CAD1ED4A5AED0, 0x137EEEBBAD824A2C, 0x57F9031E4B1202A3, 0x6BF0DC906EA79BFE, 0x758A7BDB4382A822, 0x75E5141D14732CA, 0x20A505D8E4179A25, 0xF606FA83CD832E08, 0x8F2F290477FB76A8, 0x4ACC41632D9B6888, 0xC47BCBD6D39CD877, 0xED2E110A706FA25E, 0xBC7FC792DDA3FF6A, 0x19CB37FCB0952804, 0x421AE1367E51461A, 0x1DB711BF2C18F8A2, 0x6386737A42D7FFAC, 0xEF983B2EF79B846E, 0xA0D2ECE3B084EDF3, 0x2C684993475BBC26, 0x972FEAB11949D7E7, 0x4C57C05CCB204136, 0xC370F106668E552, 0x299A4206A80694D2, 0x88637B756AE98C0A, 0xA5A155ABEBD97EDA, 0x471E84235EBB818A    
};
constexpr UInt64 BISHOP_MAGIC_NUMBERS[64] = {
    0x70768F37C675A06D, 0x8EC0A7580D0F09CC, 0x62D3470073945EFC, 0xF322890CA6693DF9, 0xD20C53E59E752BB6, 0x99CA12F519E356E0, 0xFD5CCDA5498A9700, 0x221C3958BC06D9CB, 0x758A1634BA58CF5A, 0xBD2F57A402CE941E, 0x2A3A12C012352F99, 0x32F09E216583AC23, 0x8961CF41E7CC57D9, 0x592A9E6B11FC6278, 0xE4069A1A7C977117, 0xFB7A9914E36327D5, 0xF22EE1A522BABB2, 0x9A870E85C6E84E70, 0xFD334DCD79713398, 0xB5940AA4A65DF03F, 0x8F3E090E976B6B16, 0xA7C7C324C4EF552C, 0xE2F40717A0DAD9EE, 0x29B37F4C0F550D12, 0xCA3D2E9C7A7292B6, 0xA519D8E5E383607C, 0x6CE2B226517830C0, 0x1272728DDA80CE9, 0xC3C52281E24A07D0, 0x68E66CE2939DD3FC, 0x927C61B0ED529F51, 0xE51C0300BF3B9126, 0x12CC861C3A0A3D44, 0x3C191A7B0539714E, 0x1C54ED56937C115E, 0xD8E0C2C00E5B1866, 0x4682419DE7E58BBE, 0xB39623EAC53218CE, 0xD9A6078A7B660338, 0xFF51C3734D2F7CBD, 0x374C5E932FF14A0, 0xB0B2DEC65E94AA22, 0xAE3553C01203E4B5, 0x41C6E3E630C47205, 0xC5BEBE034BF3053F, 0x3DC15FD9F97CCE69, 0x1CBFBAD4BA8883E8, 0xDF4EDA96325EC87B, 0x390E129F9A0C05F3, 0x2308B47A08ABA993, 0x7E510E58C4143D87, 0xB375DC21DDF99C39, 0x34C9D8B56237BF3E, 0x294694F454262E49, 0x4AC9AE71F4A04625, 0xDB93414774824D19, 0x83B16948DC201222, 0xFA4E07C424888926, 0x3476CAFC932AE039, 0xDBFA81A47C030564, 0xD13C4529F891BA48, 0x27D8AF18ABD2015B, 0x503CF896175C05BB, 0x19699E2CFAB0040B
};

// lsb and msb functions
int lsb(UInt64);

int msb(UInt64);

// pop count function
int popCount(UInt64);

// display bitboard
void display(UInt64);

// functions for processing files for ROOK_MAGIC_ATTACK_TABLE and BISHOP_MAGIC_ATTACK_TABLE attack masks
void processRookAttacks(std::string);

void processBishopAttacks(std::string);

// get magic index
UInt64 getMagicIndex(UInt64, UInt64, int);

// look up sliding piece attacks
UInt64 lookupRookAttack(int, UInt64);

UInt64 lookupBishopAttack(int, UInt64);

UInt64 lookupQueenAttack(int, UInt64);