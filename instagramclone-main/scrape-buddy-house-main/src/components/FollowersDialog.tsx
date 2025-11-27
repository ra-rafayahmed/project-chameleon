import { Dialog, DialogContent, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Button } from '@/components/ui/button';
import { X } from 'lucide-react';
import { getUsers, getCurrentUser } from '@/lib/localStorage';
import { Link } from 'react-router-dom';
import { useState } from 'react';
import { toast } from 'sonner';

interface FollowersDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  type: 'followers' | 'following';
  username: string;
}

export const FollowersDialog = ({ open, onOpenChange, type, username }: FollowersDialogProps) => {
  const users = getUsers();
  const currentUsername = getCurrentUser();
  const followData = JSON.parse(localStorage.getItem('followData') || '{}');
  
  // Get list of usernames based on type
  const userList = type === 'followers' 
    ? (followData[username]?.followers || [])
    : (followData[username]?.following || []);
  
  // Filter users to show
  const displayUsers = users.filter(u => userList.includes(u.username));
  
  // State to track following status for each user
  const [followingStates, setFollowingStates] = useState<Record<string, boolean>>(() => {
    const currentUserFollowing = followData[currentUsername]?.following || [];
    const states: Record<string, boolean> = {};
    displayUsers.forEach(u => {
      states[u.username] = currentUserFollowing.includes(u.username);
    });
    return states;
  });

  const handleFollowToggle = (targetUsername: string) => {
    const followData = JSON.parse(localStorage.getItem('followData') || '{}');
    
    if (!followData[currentUsername]) {
      followData[currentUsername] = { following: [], followers: [] };
    }
    if (!followData[targetUsername]) {
      followData[targetUsername] = { following: [], followers: [] };
    }
    
    const isCurrentlyFollowing = followingStates[targetUsername];
    
    if (!isCurrentlyFollowing) {
      // Follow
      if (!followData[currentUsername].following.includes(targetUsername)) {
        followData[currentUsername].following.push(targetUsername);
      }
      if (!followData[targetUsername].followers.includes(currentUsername)) {
        followData[targetUsername].followers.push(currentUsername);
      }
      toast.success(`Following ${targetUsername}`);
    } else {
      // Unfollow
      followData[currentUsername].following = followData[currentUsername].following.filter(
        (u: string) => u !== targetUsername
      );
      followData[targetUsername].followers = followData[targetUsername].followers.filter(
        (u: string) => u !== currentUsername
      );
      toast.success(`Unfollowed ${targetUsername}`);
    }
    
    localStorage.setItem('followData', JSON.stringify(followData));
    setFollowingStates(prev => ({
      ...prev,
      [targetUsername]: !isCurrentlyFollowing
    }));
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-md">
        <DialogHeader>
          <div className="flex items-center justify-between">
            <DialogTitle className="capitalize">{type}</DialogTitle>
            <Button
              variant="ghost"
              size="icon"
              onClick={() => onOpenChange(false)}
            >
              <X className="h-5 w-5" />
            </Button>
          </div>
        </DialogHeader>

        <div className="max-h-96 overflow-y-auto">
          {displayUsers.length === 0 ? (
            <p className="text-center text-muted-foreground py-8">
              No {type} yet
            </p>
          ) : (
            <div className="space-y-3">
              {displayUsers.map((user) => {
                const isOwnProfile = user.username === currentUsername;
                const isFollowing = followingStates[user.username];
                
                return (
                  <div key={user.username} className="flex items-center justify-between">
                    <Link 
                      to={`/profile/${user.username}`}
                      className="flex items-center gap-3 hover:opacity-80 transition-opacity flex-1"
                      onClick={() => onOpenChange(false)}
                    >
                      <Avatar className="h-11 w-11">
                        <AvatarImage src={user.avatar} alt={user.username} />
                        <AvatarFallback>{user.username[0].toUpperCase()}</AvatarFallback>
                      </Avatar>
                      <div className="flex-1">
                        <p className="text-sm font-semibold text-foreground">{user.username}</p>
                        <p className="text-sm text-muted-foreground">{user.fullName}</p>
                      </div>
                    </Link>
                    {!isOwnProfile && (
                      <Button 
                        variant={isFollowing ? "secondary" : "default"}
                        size="sm"
                        onClick={() => handleFollowToggle(user.username)}
                      >
                        {isFollowing ? 'Following' : 'Follow'}
                      </Button>
                    )}
                  </div>
                );
              })}
            </div>
          )}
        </div>
      </DialogContent>
    </Dialog>
  );
};
