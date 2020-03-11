package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.CONST;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.FriendApplyDTO;
import cn.edu.seu.alumni_server.controller.dto.FriendDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.controller.dto.enums.FriendStatus;
import cn.edu.seu.alumni_server.controller.dto.enums.MessageType;
import cn.edu.seu.alumni_server.dao.entity.Friend;
import cn.edu.seu.alumni_server.dao.mapper.FriendMapper;
import cn.edu.seu.alumni_server.dao.mapper.V2ApiMapper;
import cn.edu.seu.alumni_server.service.MessageService;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.*;
import tk.mybatis.mapper.entity.Example;

import javax.servlet.http.HttpServletRequest;
import java.util.List;
import java.util.Map;

/**
 * 好友逻辑
 * 申请、拒绝、查询等
 */
@RestController
@Acl
@SuppressWarnings("ALL")
public class FriendManageController {
    @Autowired
    FriendMapper friendMapper;

    @Autowired
    MessageService messageService;

    @Autowired
    V2ApiMapper v2ApiMapper;

    @PostMapping("/friend/apply")
    @Transactional
    public WebResponse friendApply(@RequestBody FriendApplyDTO friendApplyDTO) {
        Long accountId = (Long) request.getAttribute(CONST.ACL_ACCOUNTID);
        Friend f = new Friend();
        f.setAccountId(accountId);
        f.setFriendAccountId(friendApplyDTO.getFriendAccountId());


        Friend fRes = friendMapper.selectOne(f);
        if (fRes != null && fRes.getStatus() == FriendStatus.apply.getStatus()) {
            return new WebResponse().fail("请勿重复申请");
        }

        Friend A2B = new Friend();
        A2B.setAccountId(accountId);
        A2B.setFriendAccountId(friendApplyDTO.getFriendAccountId());
        A2B.setStatus(FriendStatus.apply.getStatus());

        friendMapper.insertOnDuplicateKeyUpdate(A2B);

        Friend B2A = new Friend();
        B2A.setAccountId(friendApplyDTO.getFriendAccountId());
        B2A.setFriendAccountId(accountId);
        B2A.setStatus(FriendStatus.todo.getStatus());
        friendMapper.insertOnDuplicateKeyUpdate(B2A);

        //发消息
        messageService.newMessage(accountId, friendApplyDTO.getFriendAccountId(),
                MessageType.APPLY.getValue(),"", friendApplyDTO.getMessage());

        return new WebResponse();
    }

    @PostMapping("/friend/manage")
    @Transactional
    public WebResponse friendAction(@RequestBody Map<String, Long> req) {

        Long A = req.get("A");
        Long B = req.get("B");

        if (A == null || B == null) {
            return new WebResponse().fail("处理人id A,B 不能为null");
        }


        if (req.get("action") == CONST.FRIEND_ACTION_Y) {
            Friend f = new Friend();
            f.setStatus(FriendStatus.friend.getStatus());

            // 更新两个人的好友关系
            Example e1 = new Example(Friend.class);
            e1.createCriteria()
                    .andEqualTo("accountId", req.get("B"))
                    .andEqualTo("friendAccountId", req.get("A"));
            friendMapper.updateByExampleSelective(f, e1);

            Example e2 = new Example(Friend.class);
            e2.createCriteria()
                    .andEqualTo("accountId", req.get("A"))
                    .andEqualTo("friendAccountId", req.get("B"));
            friendMapper.updateByExampleSelective(f, e2);

//            Message message = new Message();
//            message.setMessageId(Utils.generateId());
//            message.setFromUser(req.get("A"));
//            message.setToUser(req.get("B"));
//            message.setType(MessageType.AGREE.getValue());
//            messageMapper.insertSelective(message);

            messageService.newMessage(req.get("A"), req.get("B"),
                    MessageType.AGREE.getValue());
        }

        if (req.get("action") == CONST.FRIEND_ACTION_N) {
            Friend f = new Friend();

            // B 查看A的名片，还是申请中状态
            f.setStatus(FriendStatus.apply.getStatus());
            Example e1 = new Example(Friend.class);
            e1.createCriteria()
                    .andEqualTo("accountId", req.get("B"))
                    .andEqualTo("friendAccountId", req.get("A"));
            friendMapper.updateByExampleSelective(f, e1);

            // A查看B的名片，正常的陌生人状态
            f.setStatus(FriendStatus.stranger.getStatus());
            Example e2 = new Example(Friend.class);
            e2.createCriteria()
                    .andEqualTo("accountId", req.get("A"))
                    .andEqualTo("friendAccountId", req.get("B"));
            friendMapper.updateByExampleSelective(f, e2);
            /**
             * 1、拒绝机制变更
             * 现象：A拒绝B的好友申请后，B会收到通知（消息模块上线的情况下）
             * 调整为：A拒绝B的好友申请后，B不会收到任何通知，且A的名片对于B一直保持“已申请”的状态。
             * 原因：不引导用户间发生负面反馈
             */
//            Message message = new Message();
//            message.setMessageId(Utils.generateId());
//            message.setFromUser(req.get("A"));
//            message.setToUser(req.get("B"));
//            message.setType(MessageType.REJECT.getValue());
//            messageMapper.insertSelective(message);
        }

        return new WebResponse();
    }

    @Autowired
    HttpServletRequest request;

    @GetMapping("/friends")
    public WebResponse getFriends(@RequestParam int pageIndex,
                                  @RequestParam int pageSize) {
        Long accountId = (Long) request.getAttribute("accountId");

        PageHelper.startPage(pageIndex, pageSize);
        List<FriendDTO> friends = friendMapper.getFriends(accountId);

        return new WebResponse().success(new PageResult(((Page) friends).getTotal(), friends));
    }


}
