import { Heart, UserPlus, MessageCircle } from 'lucide-react';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { Button } from '@/components/ui/button';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { ScrollArea } from '@/components/ui/scroll-area';

const notifications = [
  {
    id: '1',
    type: 'like',
    user: 'john_photo',
    avatar: '/placeholder.svg',
    message: 'liked your post',
    time: '2h ago',
  },
  {
    id: '2',
    type: 'follow',
    user: 'sarah_food',
    avatar: '/placeholder.svg',
    message: 'started following you',
    time: '5h ago',
  },
  {
    id: '3',
    type: 'comment',
    user: 'mike_fitness',
    avatar: '/placeholder.svg',
    message: 'commented on your post',
    time: '1d ago',
  },
  {
    id: '4',
    type: 'like',
    user: 'david_tech',
    avatar: '/placeholder.svg',
    message: 'liked your story',
    time: '2d ago',
  },
];

export const NotificationsDropdown = () => {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger asChild>
        <Button variant="ghost" size="icon" className="relative">
          <Heart className="h-5 w-5" />
          <span className="absolute top-1 right-1 h-2 w-2 bg-red-500 rounded-full" />
        </Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent align="end" className="w-80">
        <div className="p-2 font-semibold border-b">Notifications</div>
        <ScrollArea className="h-96">
          {notifications.map((notification) => (
            <DropdownMenuItem key={notification.id} className="flex items-center gap-3 p-3 cursor-pointer">
              <Avatar className="h-10 w-10">
                <AvatarImage src={notification.avatar} />
                <AvatarFallback>{notification.user[0].toUpperCase()}</AvatarFallback>
              </Avatar>
              <div className="flex-1">
                <p className="text-sm">
                  <span className="font-semibold">{notification.user}</span> {notification.message}
                </p>
                <p className="text-xs text-muted-foreground">{notification.time}</p>
              </div>
              {notification.type === 'like' && <Heart className="h-4 w-4 text-red-500 fill-red-500" />}
              {notification.type === 'follow' && <UserPlus className="h-4 w-4 text-primary" />}
              {notification.type === 'comment' && <MessageCircle className="h-4 w-4 text-primary" />}
            </DropdownMenuItem>
          ))}
        </ScrollArea>
      </DropdownMenuContent>
    </DropdownMenu>
  );
};
