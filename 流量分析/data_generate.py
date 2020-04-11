# coding=utf-8
try:
    import scapy_http.http
except ImportError:
    from scapy.layers import http
try:
    from scapy.all import Raw,rdpcap
except ImportError:
    import scapy
import json
import time
import gc
import numpy as np

iptoNum = lambda x:sum([256**j*int(i) for j,i in enumerate(x.split('.')[::-1])])

# reorder the out of order tcp seq
def function(youxu = [], shixu = []):
    kong = []
    if len(shixu) == 0:
        return youxu, kong
    for i in range(len(shixu)):
        next_seq = shixu[i][1]
        next_len = shixu[i][2]
        pre_seq = youxu[-1][1]
        pre_len = youxu[-1][2]
        if next_seq == (pre_seq + pre_len):
            youxu.append(shixu[i])
        elif next_seq > (pre_seq + pre_len):
            kong.append(shixu[i])
        elif (next_seq == pre_seq) & (next_len < pre_len):
            continue
        else:
            continue
    return youxu,kong

# process the out of order of tcp seq
def reorder_seq(daoda = []):
    youxu = []
    shixu = []
    packet_order = []
    #print(proper_order)
    youxu.append(daoda[0])
    #print(youxu)
    for s in range(1, len(daoda)):
        next_seq = daoda[s][1]
        next_len = daoda[s][2]
        pre_seq = youxu[-1][1]
        pre_len = youxu[-1][2]
        if next_seq == (pre_seq + pre_len):
            youxu.append(daoda[s])
        elif next_seq > (pre_seq + pre_len):
            shixu.append(daoda[s])
        elif (next_seq == pre_seq) & (next_len < pre_len):
            continue
        else:
            continue
    # put the shixu into the youxu list
    count = 0
    #print('youxu:   ',youxu)
    #print('shixu:   ',shixu)
    if len(shixu) == 0:
        for s in range(len(youxu)):
            packet_order.append(youxu[s][0])
        return packet_order
    while True:
        count += 1
        #print(count)
        you, shi = function(youxu, shixu)
        if (len(shi) == 0):
            break
        youxu = you
        shixu = shi
    # insert the correct order into the real_item_list[j]['item']
    for s in range(len(you)):
        packet_order.append(you[s][0])
    return packet_order

def select_tagflow():
    tagFile = 'edge-net-export-log.json'
    url_ip = {'src':'','dst':''}
    idle_list = []
    url_ip_list = []
    url_ip6_list = []
    gc.collect()
    
    tag_fp = open(tagFile, 'r',encoding='utf-8')
    url_py = json.load(tag_fp)
    '''for key in url_py['polledData']['socketPoolInfo'][0]['groups'].keys():
        url_item = url_py['polledData']['socketPoolInfo'][0]['groups'][key]
        idle_list = url_item['idle_sockets']
        print(key)''' 

    #select out the connectionID list
    for item in url_py['events']:
        if 'params' in item.keys() and item['type'] == 34: #source_dependency'in item['params'].keys() and item['params']['source_dependency']['type'] == 6 and''' 
            idle_list.append(item['source']['id'])
    #
    for idle_no in idle_list:
        flag = True
        for it in url_py['events']:
            if 'params' in it.keys() and it['source']['id'] == idle_no:
                if('address' in it['params'].keys()):
                    dst_ip = it['params']['address']
                else:
                    if('source_address' in it['params'].keys()):
                        src_ip = it['params']['source_address']
                        flag = False
                        break
                    else:
                        continue
        if flag:
            continue
        if '[' == src_ip[0]: #this couple of ip is ipv6: src_ip='[2401:b180:2000:20::21]:80'
            #ex_src6 = src_ip.split(']:')
            #ex_dst6 = dst_ip.split(']:')
            url_ip = {'src':src_ip,'dst':dst_ip}
            url_ip6_list.append(url_ip)
            #print(url_ip)
        else:
            #print(src_ip)
            #ex_src = src_ip.split(':')
            #ex_dst = dst_ip.split(':')
            url_ip = {'src':src_ip,'dst':dst_ip} 
            url_ip_list.append(url_ip) 
            print(url_ip)  
    # ipv4 5 tuples in url_ip_list
    # ipv6 5 tuples in url_ip6_list   
    tag_fp.close()
    return url_ip_list, url_ip6_list

def select_flow():  
    # select_flow in the pcap_file
    ip_list, ip6_list = select_tagflow()
    pkts = rdpcap('dump.pcap') 
    item_list = []                       # record the all the stream, including bi-directions
    real_item_list = []                  # two transform into one direction
    session_dict = {'src':'','dst':'','item':[],'tag':0}
    # flag all sessions' packets and save to list 
    # the session define to bi-direction TCP 
    for ses in pkts.sessions():      
        L = ses.split()
        if L[0] != 'TCP':               # record all the tcp sessions
            continue
        session_dict = {'src':L[1],'dst': L[3],'item':[],'upload':[],'tag':0}
        item_list.append(session_dict)
    # del the duplicate session
    del_pos = []
    for i in range(len(item_list)):
        for j in range(i+1, len(item_list)):
            if item_list[i]['src'] == item_list[j]['dst'] and item_list[i]['dst'] == item_list[j]['src']:
                del_pos.append(j)
    for i in range(len(item_list)):
        if i in del_pos:
            continue
        real_item_list.append(item_list[i])
    
    #start to tag the web traffic flow ipv4
    for i in range(len(real_item_list)):
        temp1 = {'src':real_item_list[i]['src'],'dst':real_item_list[i]['dst']}
        temp2 = {'src':real_item_list[i]['dst'],'dst':real_item_list[i]['src']}
        if temp1 in ip_list or temp2 in ip_list:
            real_item_list[i]['tag'] = 1
        if temp1 in ip6_list or temp2 in ip6_list:
            real_item_list[i]['tag'] = 1
        #print(real_item_list[i])
     
    # to grab the postion of the fisrt 20 packet 此处的位置是从0开始的
    for j in range(len(real_item_list)):
        packet_20 = 0
        for i in range(len(pkts)):
            #if packet_20 == 50:
                #break
            if(pkts[i].payload.name == 'IP'):
                sip = pkts[i].payload.src
                dip = pkts[i].payload.dst
                if(pkts[i].payload.payload.name == 'TCP'):
                    sport = pkts[i].payload.payload.sport
                    dport = pkts[i].payload.payload.dport
                    #start to match in real_item_list = {}
                    ip_port1 = sip+':'+str(sport)
                    ip_port2 = dip+':'+str(dport)
                    #print(ip_port1,ip_port1)
                    if real_item_list[j]['src'] == ip_port1 and real_item_list[j]['dst'] == ip_port2:
                        #print(real_item_list[j]['src'],ip_port1)
                        real_item_list[j]['item'].append(i)
                        packet_20 += 1
                    elif real_item_list[j]['dst'] == ip_port1 and real_item_list[j]['src'] == ip_port2:
                        real_item_list[j]['item'].append(i)
                        packet_20 += 1
            else:
                continue 
    # to grab the packet length of the each session in the item[]['item']
    #对丢包数据重传数据进行处理

    #load_1_flaglist = []
    #for j in range(len(real_item_list)):
    #    if real_item_list[j]['tag'] == 1:
    #        load_1_flaglist.append(real_item_list[j])
    #del load_1_flaglist[0]
    #del load_1_flaglist[-1]
    #real_item_list = [{'src': '192.168.0.101:60173', 'dst': '101.227.172.14:443', 'item': [485, 486, 487, 498, 501, 502, 504, 505, 506, 507, 508, 510, 511, 512, 513, 515, 517, 518, 520, 521, 523, 524, 525, 527, 529, 530, 532, 533, 535, 536, 538, 539, 541, 542, 1100, 1102, 1109, 1205, 1206, 1992, 1993, 1994, 1995, 1996, 1998, 1999, 2001, 2006, 2007, 2009, 2011, 2012, 2013, 2057, 2237, 2151, 2152, 2153, 2155, 2156, 2157, 2158, 2159, 2165, 2166, 2167, 2168, 2169, 2170, 2171, 2192, 2193, 2194, 2195, 2196, 2197, 2198, 2199, 2212, 2213, 2216, 2217, 2218, 2222, 2224, 2225, 2226, 2696, 7656, 7657, 7658, 7668, 7669, 7671, 7674, 7675, 7676, 7682, 7686, 7687, 7689, 7690, 7691, 7694, 7695, 7698, 7705, 7706, 7719, 7723, 7724, 7725, 7726, 7727, 7728, 7730, 7731, 7795, 7797, 7798, 7801, 7802, 7805, 7806, 7810, 7811, 7814, 7816, 7817, 7818, 7822, 7823, 7826, 7827, 7831, 7741, 7743, 7744, 7745, 7746, 7747, 7753, 7754, 7757, 7759, 7760, 7761, 7765, 7766, 7767, 7771, 7772, 7909, 7910, 7912, 7913, 7915, 7916, 7918, 7919, 7920, 7922, 7924, 7925, 7930, 7931, 7932, 7934, 7955, 7957, 7958, 7960, 7961, 7962, 7964, 7965, 7966, 7968, 7969, 7970, 7971, 7973, 7974, 7975, 7977, 7978, 7979, 7981, 7982, 7983, 7984, 7986, 7987, 7989, 7991, 7992, 7994, 7997, 7998, 7999, 8000, 8001, 8002, 8004, 8005, 8006, 8007, 8014, 8016, 8018, 8019, 8021, 8022, 8023, 8024, 8026, 8027, 8028, 8029, 8030, 8031, 8032, 8033, 8044, 8046, 8047, 8049, 8050, 8051, 8053, 8054, 8055, 8056, 8057, 8058, 8059, 8060, 8061, 8063, 8064, 8065, 8066], 'tag': 1}]
    #print(real_item_list[25])
    print('进入重新排序')
    for j in range(len(real_item_list)):
        '''if '61757' in real_item_list[j]['src']:
            print(real_item_list[j],j)'''
        if real_item_list[j]['tag'] == 1:
            daoda = []
            packet_list = real_item_list[j]['item']
            up = []
            if len(packet_list) == 0:
                continue
            for i in packet_list:
                packet_len = pkts[i].payload.len
                ip_len = pkts[i].ihl
                tcp_len = pkts[i].dataofs
                if (packet_len - 4*(ip_len + tcp_len)) == 0:
                    continue
                if (packet_len == 40) | ((packet_len == 52)&(pkts[i].payload.payload.flags == 18)) | (packet_len < 43):   # leave the ack and tcp handshakes and retransmissio
                    #proper_order.append(i)
                    continue
                if '192' in pkts[i].payload.src:
                    up.append(i)
                    continue
                raw_length = pkts[i].len - (pkts[i].ihl + pkts[i].dataofs) * 4
                daoda.append([i, pkts[i].seq, raw_length])
            if len(daoda) == 0:
                continue
            
            li = reorder_seq(daoda)
            real_item_list[j]['item'] = li 
            real_item_list[j]['upload'] = up
    print('结束重新排序')
    count = 0
    tls_stream = []
    for j in range(len(real_item_list[:])):
        if real_item_list[j]['tag'] == 1:
            count += 1
            #tls_stream.append('No:'+str(count)+'|||')
            data_stream = []
            total_tls_length = 0
            packet_li = real_item_list[j]['item']
            tool_list = []
            tool_flag = False
            for i in packet_li:
                
                #print(type(i), j)
                #print(len(tls_stream))
                #print(total_tls_length)
                direction_flag = 1
                if '192' in pkts[i].payload.src:
                    direction_flag = -1
                packet_len = pkts[i].payload.len
                if (packet_len == 40) | ((packet_len == 52)&(pkts[i].payload.payload.flags == 18)) | (packet_len < 43):   # leave the ack and tcp handshakes and retransmissio
                    continue
                if pkts[i].haslayer(Raw): #|(i == 1125)|(i == 1126)
                    #print('包的位置是'+str(i))
                    if tool_flag:
                        tool_flag = False
                        pre_len = len(tool_list)
                        if pre_len == 1:
                            total_tls_length = pkts[i]['Raw'].load[2]*16**2+pkts[i]['Raw'].load[3]
                        elif pre_len == 2:
                            total_tls_length = pkts[i]['Raw'].load[1]*16**2+pkts[i]['Raw'].load[2]
                        elif pre_len == 3:
                            total_tls_length = pkts[i]['Raw'].load[0]*16**2+pkts[i]['Raw'].load[1]
                        elif pre_len == 4:
                            total_tls_length = tool_list[3]*16**2+pkts[i]['Raw'].load[0]
                        tool_list = []
                        realraw_length = pkts[i].payload.len - 4 * (pkts[i].ihl+pkts[i].dataofs)
                        if total_tls_length != 0:
                            if (total_tls_length - realraw_length) >= 0: #
                                data_stream.append(realraw_length * direction_flag)
                                #print('2包的位置是'+str(i+1),total_tls_length)
                                total_tls_length -= realraw_length
                            else: 
                                # multi_tls record in the same packet
                                accu_len = total_tls_length + (5 - pre_len)
                                data_stream.append(total_tls_length * direction_flag)
                                if 0 < (realraw_length - accu_len) < 5: # only one tls inside
                                    yuxia = realraw_length - total_tls_length
                                    tool_flag = True
                                    for z in range(yuxia):                          
                                        tool_list.append(pkts[i]['Raw'].load[total_tls_length+z])
                                    total_tls_length = 0
                                else: # more than one tls inside
                                    tls_record_list = []
                                    while accu_len < realraw_length:
                                        total_tls_length = pkts[i]['Raw'].load[accu_len+3]*16**2+pkts[i]['Raw'].load[accu_len+4] 
                                        if direction_flag == 1:
                                            tls_stream.append(total_tls_length)
                                        if 0 < (realraw_length - (accu_len + total_tls_length +5)) < 5:
                                            yuxia = realraw_length - (accu_len + total_tls_length +5)
                                            tool_flag = True
                                            for z in range(yuxia):                          
                                                tool_list.append(pkts[i]['Raw'].load[accu_len+z])
                                            break
                                        elif 0 >= (realraw_length-(accu_len + total_tls_length +5)):
                                            break
                                        tls_record_list.append(total_tls_length)
                                        accu_len += (total_tls_length + 5)
                                    for i in range(len(tls_record_list)):
                                        data_stream.append(tls_record_list[i] * direction_flag)
                                    if tool_flag:
                                        total_tls_length = 0
                                    else:
                                        tls_length = realraw_length - accu_len -5
                                        data_stream.append(tls_length * direction_flag)
                                        total_tls_length -= tls_length  
                            continue
                    if (pkts[i]['Raw'].load[0] == 23) & (pkts[i]['Raw'].load[1] == 3) & (pkts[i]['Raw'].load[2] == 3):
                        total_tls_length = pkts[i]['Raw'].load[3]*16**2+pkts[i]['Raw'].load[4]
                        if direction_flag == 1 :
                            tls_stream.append(total_tls_length)
                        raw_length = pkts[i].payload.len - 4 * (pkts[i].ihl+pkts[i].dataofs) - 5 # the length of one packet
                        if (total_tls_length - raw_length) >= 0:
                            data_stream.append(raw_length * direction_flag)
                            #print('0包的位置是'+str(i+1), total_tls_length)
                            total_tls_length -= raw_length
                            #print(total_tls_length)
                        else: # multi tls record in one packet 
                            #print('1包的位置是'+str(i+1), total_tls_length)
                            tls_record_list = []
                            tls_record_list.append(total_tls_length)
                            accu_tls_len = total_tls_length + 5
                            while accu_tls_len != raw_length:
                                total_tls_length = pkts[i]['Raw'].load[accu_tls_len+3]*16**2+pkts[i]['Raw'].load[accu_tls_len+4] 
                                if direction_flag == 1:
                                    tls_stream.append(total_tls_length)
                                if (accu_tls_len + total_tls_length +5) > raw_length:
                                    break
                                tls_record_list.append(total_tls_length)
                                accu_tls_len += (total_tls_length + 5)
                            for i in range(len(tls_record_list)):
                                data_stream.append(tls_record_list[i] * direction_flag)
                            tls_length = raw_length - accu_tls_len
                            data_stream.append(tls_length * direction_flag)
                            total_tls_length -= tls_length
                            #print(total_tls_length)
                    elif total_tls_length != 0:
                        realraw_length = pkts[i].payload.len - 4 * (pkts[i].ihl+pkts[i].dataofs)
                        #print(realraw_length)
                        #print(total_tls_length)
                        if (total_tls_length - realraw_length) >= 0: #
                            data_stream.append(realraw_length * direction_flag)
                            #print('2包的位置是'+str(i+1),total_tls_length)
                            total_tls_length -= realraw_length
                        else: 
                            # multi_tls record in the same packet
                            accu_len = total_tls_length
                            data_stream.append(total_tls_length * direction_flag)

                            #print('3包的位置是'+str(i+1),total_tls_length,realraw_length)
                            if 0 < (realraw_length - accu_len) < 5: # only one tls inside
                                yuxia = realraw_length - total_tls_length
                                tool_flag = True
                                for z in range(yuxia):                          
                                    tool_list.append(pkts[i]['Raw'].load[total_tls_length+z])
                                total_tls_length = 0
                            else: # more than one tls inside
                                tls_record_list = []
                                while accu_len < realraw_length:
                                    total_tls_length = pkts[i]['Raw'].load[accu_len+3]*16**2+pkts[i]['Raw'].load[accu_len+4] 
                                    #print('3情况中的total_len'+str(total_tls_length))
                                    #print('3情况中的accu_len'+str(accu_len))
                                    if direction_flag == 1:
                                        tls_stream.append(total_tls_length)
                                    if 0 < (realraw_length - (accu_len + total_tls_length +5)) < 5:
                                        yuxia = realraw_length - (accu_len + total_tls_length +5)
                                        tool_flag = True
                                        for z in range(yuxia):                          
                                            tool_list.append(pkts[i]['Raw'].load[accu_len+z])
                                        break
                                    elif 0 >= (realraw_length-(accu_len + total_tls_length +5)):
                                        break
                                    tls_record_list.append(total_tls_length)
                                    accu_len += (total_tls_length + 5)
                                for i in range(len(tls_record_list)):
                                    data_stream.append(tls_record_list[i] * direction_flag)
                                if tool_flag:
                                    total_tls_length = 0
                                else:
                                    tls_length = realraw_length - accu_len -5
                                    data_stream.append(tls_length * direction_flag)
                                    total_tls_length -= tls_length
            #print('\n第',count,'个connection的数据流：',data_stream)
    print('\n\n\n                 _________________________客户端接收到的TLS 分包大小                  ')
    print(tls_stream)
    # 处理上行的数据
    upload_tls = []
    for j in range(len(real_item_list)):
        if real_item_list[j]['tag'] == 1:
            packet_li = real_item_list[j]['upload']
            for i in packet_li:
                if pkts[i].haslayer(Raw):
                    if (pkts[i]['Raw'].load[0] == 23) & (pkts[i]['Raw'].load[1] == 3) & (pkts[i]['Raw'].load[2] == 3):
                        total_tls_length = pkts[i]['Raw'].load[3]*16**2+pkts[i]['Raw'].load[4]
                        upload_tls.append(total_tls_length)
    print('\n\n\n                 _________________________上行TLS 分包大小                  ')
    print(upload_tls)                    
    download = np.array(tls_stream)
    np.save('download',download)
    #print(real_item_list)

if __name__ == '__main__':
    select_flow()


    '''elif 0 < (realraw_length - total_tls_length) < 5:
                            print('3包的位置是'+str(i+1),total_tls_length)
                            data_stream.append(realraw_length * direction_flag)
                            yuxia = realraw_length - total_tls_length
                            tool_flag = True
                            for z in range(yuxia):                          
                                tool_list.append(pkts[i]['Raw'].load[total_tls_length+z])
                            total_tls_length = 0'''