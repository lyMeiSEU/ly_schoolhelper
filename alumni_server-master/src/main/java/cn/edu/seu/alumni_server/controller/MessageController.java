package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.controller.dto.MessageDTO;
import cn.edu.seu.alumni_server.controller.dto.PageResult;
import cn.edu.seu.alumni_server.dao.entity.Message;
import cn.edu.seu.alumni_server.dao.mapper.AccountMapper;
import cn.edu.seu.alumni_server.dao.mapper.MessageMapper;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import java.util.List;

/**
 * 消息列表
 */
@RestController
@SuppressWarnings("ALL")
@Acl
public class MessageController {

    @Autowired
    MessageMapper messageMapper;
    @Autowired
    AccountMapper accountMapper;
    @Autowired
    HttpServletRequest request;

    @GetMapping("/message")
    public WebResponse readMessgae(@RequestParam(required = false) Integer status,
                                   @RequestParam Integer pageIndex,
                                   @RequestParam Integer pageSize) {
        Long accountId = (Long) request.getAttribute("accountId");
        if (status != null && status != 0 && status != 1) {
            return new WebResponse().fail("status只能为0，1");
        }
        PageHelper.startPage(pageIndex, pageSize);

        Message message = new Message();
        message.setToUser(accountId);
        message.setStatus(status);
        List<MessageDTO> res = messageMapper.getMessagesByAccountId(accountId, status);

        return new WebResponse().success(
                new PageResult<MessageDTO>(((Page) res).getTotal(), res));
    }

    @PostMapping("/message/changeStatus")
    public WebResponse readMessgae(@RequestBody MessageDTO messageDTO) {
        Message message = new Message();
        message.setMessageId(messageDTO.getMessageId());
        message.setStatus(messageDTO.getStatus());
        messageMapper.updateByPrimaryKeySelective(message);
        return new WebResponse();
    }
}
