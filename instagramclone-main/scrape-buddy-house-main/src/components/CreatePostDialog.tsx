import { useState } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle } from '@/components/ui/dialog';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Textarea } from '@/components/ui/textarea';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { ImagePlus, X } from 'lucide-react';
import { useCurrentUser } from '@/hooks/useLocalStorage';
import type { Post } from '@/data/mockData';

interface CreatePostDialogProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  onPostCreated: (post: Post) => void;
}

export const CreatePostDialog = ({ open, onOpenChange, onPostCreated }: CreatePostDialogProps) => {
  const { currentUser } = useCurrentUser();
  const [image, setImage] = useState<string>('');
  const [caption, setCaption] = useState('');
  const [location, setLocation] = useState('');

  const handleImageUpload = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        setImage(reader.result as string);
      };
      reader.readAsDataURL(file);
    }
  };

  const handleSubmit = () => {
    if (!image || !currentUser) return;

    const newPost: Post = {
      id: Date.now().toString(),
      userId: currentUser.id,
      username: currentUser.username,
      userAvatar: currentUser.avatar,
      image,
      caption,
      location: location || undefined,
      likes: 0,
      comments: [],
      timestamp: 'Just now',
    };

    onPostCreated(newPost);
    setImage('');
    setCaption('');
    setLocation('');
    onOpenChange(false);
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl">
        <DialogHeader>
          <DialogTitle>Create new post</DialogTitle>
        </DialogHeader>

        <div className="space-y-4">
          {/* Image Upload */}
          {!image ? (
            <label className="flex flex-col items-center justify-center h-64 border-2 border-dashed border-border rounded-lg cursor-pointer hover:bg-muted/50 transition-colors">
              <ImagePlus className="h-12 w-12 text-muted-foreground mb-2" />
              <p className="text-sm text-muted-foreground">Click to upload image</p>
              <input
                type="file"
                accept="image/*"
                className="hidden"
                onChange={handleImageUpload}
              />
            </label>
          ) : (
            <div className="relative">
              <img src={image} alt="Upload preview" className="w-full rounded-lg" />
              <Button
                variant="destructive"
                size="icon"
                className="absolute top-2 right-2"
                onClick={() => setImage('')}
              >
                <X className="h-4 w-4" />
              </Button>
            </div>
          )}

          {/* User Info */}
          {currentUser && (
            <div className="flex items-center gap-3">
              <Avatar className="h-8 w-8">
                <AvatarImage src={currentUser.avatar} alt={currentUser.username} />
                <AvatarFallback>{currentUser.username[0].toUpperCase()}</AvatarFallback>
              </Avatar>
              <p className="font-semibold text-sm">{currentUser.username}</p>
            </div>
          )}

          {/* Caption */}
          <Textarea
            placeholder="Write a caption..."
            value={caption}
            onChange={(e) => setCaption(e.target.value)}
            rows={3}
          />

          {/* Location */}
          <Input
            placeholder="Add location (optional)"
            value={location}
            onChange={(e) => setLocation(e.target.value)}
          />

          {/* Submit Button */}
          <Button
            onClick={handleSubmit}
            disabled={!image}
            className="w-full"
          >
            Share Post
          </Button>
        </div>
      </DialogContent>
    </Dialog>
  );
};
